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
bool ignore_first_line = true;


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

  // auto values = distance_sensor_.getArrayDistance();
  // float left = values[0];
  // float right = values[1];

  // Serial.print("Left: ");
  // Serial.print(left);
  // Serial.print(" | Right: ");
  // Serial.println(right);

  switch (currentState) {
    case STATES::START:
      Serial.println("START STATE");
      if (ignore_first_line && line_sensor_.isFrontLine()) {
        drive_.acceptInput(0,70,0);
      } else {
        ignore_first_line = false;
        if (distance_sensor_.isObstacle()) {
          currentState = STATES::TURN_LEFT;
        } else {
          drive_.acceptInput(0,70,0);
          if (line_sensor_.isFrontLine()) {
            drive_.acceptInput(0,0,0);
            currentState = STATES::ENDLINE;
          }
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
      if(line_sensor_.isLeftLine()) {
        drive_.acceptInput(0,0,0);
        currentState = STATES::RIGHTMOST;
      } else {
        drive_.followFrontLineLeft();
      }
      break;

    case STATES::RIGHTMOST:
      Serial.println("Estado: RIGHTMOST");
      if(line_sensor_.isRightLine()){
        drive_.acceptInput(0,0,0);
      }else{
        drive_.followFrontLineRight();
      }
      break;

    case STATES::RETURN:
      Serial.println("Estado: RETURN");
      drive_.acceptInput(0,0,180);
      if(distance_sensor_.isObstacle() && millis() - start_time > 1500) {
        if (start_time == 0) {
          start_time = millis();
        }
        if (millis() - start_time > 500) {
          if(line_sensor_.isLeftLine()){
            drive_.acceptInput(50,0,0);
          }else{
            drive_.acceptInput(-50,0,0);
          }
        }
      } else {
        drive_.acceptInput(0,50,0);
        if (line_sensor_.isFrontLine() && (millis() - start_time > 500)) {
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
}