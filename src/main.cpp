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
  


  if(line_sensor_.isFrontLine()){
    drive_.acceptInput(0,0,0);
  }else{
      drive_.acceptInput(0,60,0);
  }
  /*
  switch (currentState) {
    case STATES::START:
    Serial.println("Estado: START");
    drive_.acceptInput(0,90,0);  

    if (distance_sensor_.isObstacle()) {
      drive_.acceptInput(0,0,0);
      
      if (start_time == 0) {
        start_time = millis();
      }
      if (millis() - start_time > 1000) {
        if(line_sensor_.isLeftLine()){
          drive_.acceptInput(90,0,0);
        }else {
          drive_.acceptInput(-90,0,0);
        }
      }
    } else {
      drive_.acceptInput(0,90,0);
      if(line_sensor_.isFrontLine() && millis() - start_time > 1500){
        drive_.acceptInput(0,0,0);
        currentState = STATES::ENDLINE;
      }
      start_time = 0;  
    }
    break;

    case STATES::ENDLINE:
      Serial.println("Estado: ENDLINE");
      if (line_sensor_.isFrontLine()) {
        drive_.acceptInput(0,0,0);
        if (millis() - start_time > 1500) {
          drive_.acceptInput(-90,0,0);
          if (line_sensor_.isLeftLine()) {
            drive_.acceptInput(0,0,0);
            currentState = STATES::RIGHTMOST;
            start_time = millis();
          }
        }
      }
      break;

    case STATES::RIGHTMOST:
      Serial.println("Estado: RIGHTMOST");
      if (line_sensor_.isRightLine() && line_sensor_.isFrontLine()) {
        drive_.acceptInput(0,0,0);
        start_time = millis();
        currentState = STATES::RETURN;
      } else {
        drive_.acceptInput(90,0,0);
      }
      break;

    case STATES::RETURN:
      Serial.println("Estado: RETURN");
      if (distance_sensor_.isObstacle()) {
        if(line_sensor_.isLeftLine()){
          drive_.acceptInput(90,0,0);
        }else{
          drive_.acceptInput(-90,0,0);
        }
      } else {
        drive_.acceptInput(0,90,180);
        if (line_sensor_.isFrontLine() && (millis() - start_time > 3000)) {
          drive_.acceptInput(0,0,180);
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