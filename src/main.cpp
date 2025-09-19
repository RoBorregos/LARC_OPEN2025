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
  TURN_LEFT,
  ENDLINE,
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

  auto values = line_sensor_.readSensors(); 
  bool frontLeft = values[0];
  bool frontRight = values[1];

  bool lineDetected = (frontLeft || frontRight);
  if(lineDetected){
    drive_.acceptInput(0,0,0);
    start_time = millis();
    if(millis() - start_time > 1500){
      drive_.followFrontLine(0);
    }
  }else if (!lineDetected && drive_.getAverageDistanceTraveled() > 100){
    drive_.acceptInput(0,50,0);
  }

  /*
  switch (currentState) {
    case STATES::START:
      Serial.println("START STATE");
      if(distance_sensor_.isObstacle()){
        drive_.acceptInput(0,0,0);
        currentState = STATES::TURN_LEFT;
      }else{
        drive_.acceptInput(0,70,0);
        if(drive_.getAverageDistanceTraveled() > 90){
          currentState = STATES::ENDLINE;
        }
      }
      break;

    case STATES::TURN_LEFT:
      Serial.println("TURN_LEFT STATE");

      if(line_sensor_.isLeftLine()){
        drive_.acceptInput(100,0,0);
      }else{
        drive_.acceptInput(-100,0,0);
      }
      if (!distance_sensor_.isObstacle()) {
        currentState = STATES::ENDLINE;
      }
      break;

    case STATES::ENDLINE:
      Serial.println("ENDLINE STATE");
      if(line_sensor_.isLeftLine() && line_sensor_.isLeftLine()) {
        drive_.acceptInput(0,0,0);
        currentState = STATES::RIGHTMOST;
      }
      else {
        drive_.followFrontLine(0);
      }
      break;

    case STATES::RIGHTMOST:
      Serial.println("Estado: RIGHTMOST");
      if(line_sensor_.isRightLine()){
        drive_.acceptInput(0,0,0);
        currentState = STATES::RETURN;
      }else{
        drive_.followFrontLine(1);
      }
      break;

    case STATES::RETURN:
      Serial.println("Estado: RETURN");
      drive_.acceptInput(0,0,180);
      if(distance_sensor_.isObstacle()) {
        if (start_time == 0) {
          start_time = millis();
        }
        if (millis() - start_time > 500) {
          if(line_sensor_.isLeftLine()){
            drive_.acceptInput(100,0,0);
          }else{
            drive_.acceptInput(-100,0,0);
          }
        }
      } else {
        drive_.acceptInput(0,50,0);
        if (line_sensor_.isFrontLine() && (millis() - start_time > 3000)) {
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
  */
}