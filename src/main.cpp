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
int line_counter = 0;

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

  drive_.followFrontLine();

  /*
  switch (currentState) {
    case STATES::START:
    Serial.println("Estado: START");
    if (distance_sensor_.isObstacle()) {
      drive_.acceptInput(0,0,0);
      
      if (start_time == 0) {
        start_time = millis();
      }
      if (millis() - start_time > 500) {
        if(line_sensor_.isLeftLine()){
          drive_.acceptInput(100,0,0);
        }else {
          drive_.acceptInput(-100,0,0);
        }
      }
    } else {
      drive_.acceptInput(0,60,0);
      if(millis() - start_time > 6000){
        currentState = STATES::ENDLINE;
      }
      start_time = 0;  
    }
    break;

    case STATES::ENDLINE:
      Serial.println("Estado: ENDLINE");
      if (line_sensor_.isFrontLine()) {
        drive_.acceptInput(0,0,0);
        if (line_sensor_.isLeftLine()) {
          drive_.acceptInput(0,0,0);
          currentState = STATES::RIGHTMOST;
        }else{
          drive_.acceptInput(-60,0,0);
        }
      }else{
        drive_.acceptInput(0,60,0);
      }
      break;

    case STATES::RIGHTMOST:
    Serial.println("Estado: RIGHTMOST");
      if (millis() - start_time > 1500) {
        drive_.acceptInput(60,0,0);
        if(line_sensor_.isRightLine()){
          drive_.acceptInput(0,0,0);
          currentState = STATES::RETURN;
        }
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