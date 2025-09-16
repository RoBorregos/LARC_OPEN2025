/**
 * LARC 2025
 *
 * @Roborregos
 */

#include <Arduino.h>
#include <Wire.h>

#include "robot/robot_instances.h"

enum class STATES
{
  START,
  ENDLINE,
  LEFTMOST,
  RIGHTMOST,
  RETURN
};

STATES currentState = STATES::START;
unsigned long start_time = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  Wire.begin();

  // All systems must begin after initializing the serial and as the code starts running
  drive_.begin();
  com_.begin();
  line_sensor_.begin();
  distance_sensor_.begin();

  drive_.setState(0);
  drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));
 
}

void loop()
{
  drive_.update();
  delay(50);

  std::vector<int> sensorValues = line_sensor_.readSensors();
  int frontLeftLine = sensorValues[0];
  int frontRightLine = sensorValues[1];
  int backLeftLine = sensorValues[2];
  int backRightLine = sensorValues[3];

  bool frontLine = frontLeftLine && frontRightLine;
  bool leftLine = frontLeftLine && backLeftLine;
  bool backLine = backLeftLine && backRightLine;
  bool rightLine = frontRightLine && backRightLine;
  
  auto distanceValues = distance_sensor_.getArrayDistance();
  int frontLeftDistance = distanceValues[0];
  int frontRightDistance = distanceValues[1];

  bool obstacle = (frontLeftDistance < 25) || (frontRightDistance < 25);

  switch (currentState) {
    case STATES::START:
    Serial.println("Estado: START");
    drive_.acceptInput(0,110,0);  

    if (obstacle) {
      if (start_time == 0) {
        start_time = millis();
      }

      drive_.acceptInput(0,0,0);

      if (millis() - start_time > 1000) {
        drive_.acceptInput(-110,0,0);
      }
    } else {
      drive_.acceptInput(0,110,0);
      currentState = STATES::ENDLINE;
      start_time = 0;  
    }
    break;

    case STATES::ENDLINE:
      Serial.println("Estado: ENDLINE");
      if (frontLine) {
        drive_.acceptInput(0,0,0);
        if (millis() - start_time > 1500) {
          drive_.acceptInput(-200,0,0);
          if (leftLine) {
            drive_.acceptInput(0,0,0);
            currentState = STATES::RIGHTMOST;
            start_time = millis();
          }
        }
      }
      break;

    case STATES::RIGHTMOST:
      Serial.println("Estado: RIGHTMOST");
      if (rightLine) {
        drive_.acceptInput(0,0,0);
        currentState = STATES::RETURN;
        start_time = millis();
      } else {
        drive_.acceptInput(110,0,0);
      }
      break;

    case STATES::RETURN:
      Serial.println("Estado: RETURN");
      drive_.acceptInput(0,0,180);
      if (obstacle) {
        drive_.acceptInput(110,0,0);
      } else {
        drive_.acceptInput(0,110,0);
        if (frontLine && (millis() - start_time > 3000)) {
          drive_.acceptInput(0,0,0);
          currentState = STATES::START;
        }
      }
      break;

    default:
      Serial.println("Error: Estado desconocido.");
      drive_.acceptInput(0,0,0);
      break;
  }
  /* Debugging
    // drive_.acceptInput(0,110,0);
    // delay(3000);
    // drive_.acceptInput(0,0,180);
    // delay(3000);


    // Imprimir todos los valores en un solo "array"
    // Serial.print("[");
    // for (size_t i = 0; i < sensorValues.size(); ++i)
    // {
    //   Serial.print(sensorValues[i]);
    //   if (i + 1 < sensorValues.size())
    //     Serial.print(", ");
    // }
    // Serial.println("]");

    // float distance1 = distanceValues[0];
    // float distance2 = distanceValues[1];
    
    // Serial.print("Distance 1: ");
    // Serial.print(distance1);
    // Serial.print(" cm");
    // Serial.print("Distance 2: ");
    // Serial.print(distance2);
    // Serial.print(" cm");
    // Serial.println();
    // Serial.println(String(obstacle));

  
    if(frontLine)
    {
      Serial.println("Front Line Detected");
    }else if (backLine)
    {
      Serial.println("Back Line Detected");

    }else if (leftLine)
    {
      Serial.println("Left Line Detected");
    }else if (rightLine)
    {
      Serial.println("Right Line Detected");
    }
  */
}