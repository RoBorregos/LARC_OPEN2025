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

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  Wire.begin();

  // All systems must begin after initializing the serial and as the code starts running
  drive_.begin();
  elevator_.begin();
  com_.begin();
  line_sensor_.begin();
  distance_sensor_.begin();
  
  drive_.setState(0),
  drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));
}

void loop()
{
  drive_.update();
  delay(50);

  drive_.moveForward(200);

  std::vector<int> sensorValues = line_sensor_.readSensors();
  int frontLeftLine  = sensorValues[0]; 
  int frontRightLine = sensorValues[1]; 
  int backLeftLine   = sensorValues[2];
  int backRightLine  = sensorValues[3]; 

  bool frontLine = frontLeftLine && frontRightLine;
  bool backLine = backLeftLine && backRightLine;
  bool leftLine = frontLeftLine && backLeftLine;
  bool rightLine = frontRightLine && backRightLine;

  std::vector<float> distanceValues = distance_sensor_.getArrayDistance();
  int frontLeftDistance = distanceValues[0];
  int frontRightDistance = distanceValues[1];

  bool obstacle = frontLeftDistance || frontRightDistance > 20;
  
  STATES currentState = STATES::START;
  float current_time = 0;
  bool running = true;
    while (running) {
      switch (currentState) {
        case STATES::START:
          Serial.println("Estado: START");
          if(current_time > 1.5){
            if(obstacle){
              drive_.acceptInput(150,0,0);
            }else{
              drive_.acceptInput(0,150,0);
              currentState = STATES::ENDLINE;
            }
          }
          break;

        case STATES::ENDLINE:
          Serial.println("Estado: ENDLINE");
          if(frontLine){
            drive_.acceptInput(-200,0,0);
            if(leftLine){
              drive_.hardBrake();
              currentState = STATES::RIGHTMOST;
            }
          }
          currentState = STATES::LEFTMOST;
          break;

        case STATES::RIGHTMOST:
          Serial.println("Estado: RIGHTMOST");
          if(leftLine){
            drive_.hardBrake();
            currentState = STATES::RETURN;
          }else{
            drive_.acceptInput(150,0,0);
          }
          break;

        case STATES::RETURN:
          Serial.println("Estado: RETURN");
          drive_.acceptInput(0,0,180);
          if(obstacle){
            drive_.acceptInput(150,0,0);
          }else{
            drive_.acceptInput(0,150,0);
            current_time = 0;
            if(frontLine && current_time > 3){
              drive_.hardBrake();
              running = false;
            }
          }
          break;
        default:
          Serial.println("Error: Estado desconocido.");
          running = false;
          break;
      }
    }

    Serial.println("Máquina de estados finalizada.\n");
  
  /* TEST CASES FOR LINE AND DISTANCE SENSOR
  distance_sensor_.getArrayDistance();
  auto line_sensor_data = line_sensor_.readSensors();
  for (int i = 0; i < line_sensor_data.size(); i++)
  {
    Serial.print(line_sensor_data[i]);
    if (i < line_sensor_data.size() - 1)
    {
      Serial.print(" ");
    }
  }
  Serial.println();
  */
}