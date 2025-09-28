/**
 * LARC 2025
 *
 * @Roborregos
 */

#include <Arduino.h>
#include <Wire.h>

#include "robot/robot_instances.h"
#include <SoftwareSerial.h>

// --- Bluetooth Setup ---
SoftwareSerial bluetooth(0, 1); // RX, TX pins for Bluetooth module

enum class STATES
{
  START, // Represents the start of the robot
  AVOID_OBSTACLE_LEFT,
  AVOID_OBSTACLE_RIGHT,
  GO_STRAIGHT,
  ENDLINE,
  RIGHTMOST,
  AVOID_OBSTACLE_LEFT_RETURN,
  AVOID_OBSTACLE_RIGHT_RETURN,
  RETURN,
  GO_BEGINNING
};

unsigned long start_time = 0;
uint8_t after_obstacle_offset = 200;

STATES currentState;

void setup()
{
  Serial.begin(9800);
  bluetooth.begin(9800);
  Serial.println("Starting...");
  Wire.begin();

  // All systems must begin after initializing the serial and as the code starts running
  drive_.begin();
  com_.begin();
  line_sensor_.begin();
  distance_sensor_.begin();
  drive_.setState(0);
  drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));
  currentState = STATES::RIGHTMOST;
}

void loop()
{
  drive_.update();

  drive_.acceptInput(0,0,0);


  /*
  // drive_.acceptInput(0,0,0);

  switch (currentState)
  {
  case STATES::START:
    Serial.println("START STATE");
    bluetooth.println("START STATE");

    drive_.acceptInput(0, 70, 0);
    if (distance_sensor_.isObstacle())
    {
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();

      if(start_time == 0){
        start_time = millis();
      }
      
      if(millis() - start_time > 2000){
        start_time = 0;
        currentState = STATES::AVOID_OBSTACLE_RIGHT;
      }
    }
    if (!distance_sensor_.obstacleInThePath())
    {
      currentState = STATES::GO_STRAIGHT;
    }
    break;

  case STATES::AVOID_OBSTACLE_LEFT:
    Serial.println("AVOID OBSTACLE LEFT STATE");
    bluetooth.println("AVOID OBSTACLE LEFT STATE");
    
    drive_.acceptInput(-75, 0, 0);

    if (line_sensor_.isLeftLine())
    {
      start_time = 0;
      currentState = STATES::AVOID_OBSTACLE_RIGHT;
    }

    if (!distance_sensor_.isObstacle() && start_time == 0)
    {
      start_time = millis();
    }
    
    if (distance_sensor_.isObstacle())
    {
      start_time = 0;
    }

    if (!distance_sensor_.isObstacle() && start_time > 0 && (millis() - start_time > after_obstacle_offset))
    {
      start_time = 0;
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();
      currentState = STATES::GO_STRAIGHT;
    }
    break;

  case STATES::AVOID_OBSTACLE_RIGHT:
    Serial.println("AVOID OBSTACLE RIGHT STATE");
    bluetooth.println("AVOID OBSTACLE RIGHT STATE");

    drive_.acceptInput(75, 0, 0);

    if (line_sensor_.isRightLine())
    {
      start_time = 0;
      currentState = STATES::AVOID_OBSTACLE_LEFT;
    }

    if (!distance_sensor_.isObstacle() && start_time == 0)
    {
      start_time = millis();
    }
    
    if (distance_sensor_.isObstacle())
    {
      start_time = 0;
    }

    if (!distance_sensor_.isObstacle() && start_time > 0 && (millis() - start_time > after_obstacle_offset))
    {
      start_time = 0;
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();
      currentState = STATES::GO_STRAIGHT;
    }
    break;

  case STATES::GO_STRAIGHT:
    Serial.println("GO_STRAIGHT STATE");
    bluetooth.println("GO_STRAIGHT STATE");

    if(start_time == 0){
      start_time = millis();
    }

    if(line_sensor_.isFrontLine()){
      drive_.acceptInput(0,0,0);
      drive_.hardBrake();
      if(millis() - start_time > 2000){
        start_time = 0;
        currentState = STATES::ENDLINE;
      }
    }else{
      drive_.acceptInput(0,70,0);
    }

    break;

  case STATES::ENDLINE:
    Serial.println("ENDLINE STATE");
    bluetooth.println("ENDLINE STATE");
    
    drive_.acceptInput(-70, 0, 0);
    if(line_sensor_.isLeftLine())
    {
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();

      if(start_time == 0){
        start_time = millis();
      }

      if(millis() - start_time > 2000){
        start_time = 0;
        currentState = STATES::RIGHTMOST;
      }
    }

    break;

  case STATES::RIGHTMOST:
    Serial.println("Estado: RIGHTMOST");
    bluetooth.println("Estado: RIGHTMOST");

    drive_.acceptInput(70, 0, 0);

    if (line_sensor_.isRightLine())
    {
      drive_.acceptInput(0, 0, 0);
      currentState = STATES::RETURN;
    }
    break;

  case STATES::RETURN:
    Serial.println("RETURN STATE");
    bluetooth.println("RETURN STATE");
    drive_.acceptInput(0, 0, 180);
    
    if (distance_sensor_.isObstacle())
    {
      currentState = STATES::AVOID_OBSTACLE_RIGHT_RETURN;
    }
    else if (!distance_sensor_.obstacleInThePath())
    {
      currentState = STATES::GO_BEGINNING;
    }
    else
    {
      drive_.acceptInput(0, 70, 180);
    }
    break;

  case STATES::AVOID_OBSTACLE_LEFT_RETURN:
    Serial.println("AVOID OBSTACLE LEFT RETURN STATE");
    bluetooth.println("AVOID OBSTACLE LEFT RETURN STATE");
    drive_.acceptInput(0, 0, 0);

    if (line_sensor_.isLeftLine())
    {
      start_time = 0;
      currentState = STATES::AVOID_OBSTACLE_RIGHT_RETURN;
    }
    else
    {
      drive_.acceptInput(-80, 0, 180);
    }

    if (start_time == 0)
    {
      start_time = millis();
    }

    if (!distance_sensor_.isObstacle() && millis() - start_time > 12000)
    {
      start_time = 0;
      drive_.acceptInput(0, 0, 180);
      drive_.hardBrake();
      currentState = STATES::GO_BEGINNING;
    }
    break;
  
  case STATES::AVOID_OBSTACLE_RIGHT_RETURN:
    Serial.println("AVOID OBSTACLE RIGHT RETURN STATE");
    bluetooth.println("AVOID OBSTACLE RIGHT RETURN STATE");
    drive_.acceptInput(0, 0, 0);

    if (line_sensor_.isRightLine())
    {
      start_time = 0;
      currentState = STATES::AVOID_OBSTACLE_LEFT_RETURN;
    }
    else
    {
      drive_.acceptInput(80, 0, 180);
    }

    if (start_time == 0)
    {
      start_time = millis();
    }

    if (!distance_sensor_.isObstacle() && millis() - start_time > 12000)
    {
      start_time = 0;
      drive_.acceptInput(0, 0, 180);
      drive_.hardBrake();
      currentState = STATES::GO_BEGINNING;
    }
    break;

  case STATES::GO_BEGINNING:
    Serial.println("GO BEGINNING STATE");
    bluetooth.println("GO BEGINNING STATE");
    drive_.acceptInput(0, 0, 0);

    if(line_sensor_.isFrontLine()){
      drive_.acceptInput(0,0,0);
      Serial.println("FOUND THE BEGINNING");
    }else{
      drive_.acceptInput(0,70,180);
    }
    break;

  default:
    Serial.println("Error: Estado desconocido.");
    drive_.acceptInput(0, 0, 0);
    break;
  }
  */
}