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
STATES currentState;

void setup()
{
  Serial.begin(9600);
  bluetooth.begin(9600);
  Serial.println("Starting...");
  Wire.begin();

  // All systems must begin after initializing the serial and as the code starts running
  drive_.begin();
  com_.begin();
  line_sensor_.begin();
  distance_sensor_.begin();
  drive_.setState(0);
  drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));
  currentState = STATES::START;
}

void loop()
{

  drive_.update();
  line_sensor_.update();

  switch (currentState)
  {
  case STATES::START:
    Serial.println("START STATE");
    bluetooth.println("START STATE");

    drive_.acceptInput(0, 80, 0);
    if (distance_sensor_.isObstacle())
    {
      drive_.acceptInput(0, 0, 0);
      currentState = STATES::AVOID_OBSTACLE_LEFT;
    }
    if (!distance_sensor_.obstacleInThePath())
    {
      currentState = STATES::GO_STRAIGHT;
    }
    break;

  case STATES::AVOID_OBSTACLE_LEFT:
    Serial.println("AVOID OBSTACLE LEFT STATE");
    bluetooth.println("AVOID OBSTACLE LEFT STATE");
    
    drive_.acceptInput(-60, 0, 0);

    if (line_sensor_.isLeftLine())
    {
      start_time = 0;
      currentState = STATES::AVOID_OBSTACLE_RIGHT;
    }

    if (start_time == 0)
    {
      start_time = millis();
    }
    
    if (!distance_sensor_.isObstacle() && millis() - start_time > 5000)
    {
      start_time = 0;
      drive_.acceptInput(0, 0, 0);
      currentState = STATES::GO_STRAIGHT;
    }
    break;

  case STATES::AVOID_OBSTACLE_RIGHT:
    Serial.println("AVOID OBSTACLE RIGHT STATE");
    bluetooth.println("AVOID OBSTACLE RIGHT STATE");
    if (line_sensor_.isRightLine())
    {
      start_time = 0;
      currentState = STATES::AVOID_OBSTACLE_LEFT;
    }
    else
    {
      drive_.acceptInput(60, 0, 0);
    }

    if (start_time == 0)
    {
      start_time = millis();
    }

    if (!distance_sensor_.isObstacle() && millis() - start_time > 5000)
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
    
    drive_.acceptInput(0, 70, 0);

    if (distance_sensor_.isTree())
    {
      Serial.println("LINE DETECTED");
      bluetooth.println("LINE DETECTED");
      
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();
      if (start_time == 0)
      {
        start_time = millis();
        while (millis() - start_time < 1000)
        {
          drive_.acceptInput(0, -90, 0);
        }
        currentState = STATES::ENDLINE;
      }
    }

    break;

  case STATES::ENDLINE:
  Serial.println("ENDLINE STATE");
    // if (line_sensor_.isLeftLine() || line_sensor_.isFrontLine())
    // {
    //   drive_.acceptInput(0, 0, 0);
    //   // currentState = STATES::RIGHTMOST; // esto esta mals
    // }
    // else
    // {
    //     drive_.acceptInput(-60, 0, 0);

    // }

    if(line_sensor_.isLeftLine()){
      drive_.acceptInput(0,0,0);
      currentState = STATES::RIGHTMOST;
    }else{
      drive_.acceptInput(-60,0,0);
    }

    break;

  case STATES::RIGHTMOST:
    Serial.println("Estado: RIGHTMOST");
    if (line_sensor_.isRightLine())
    {
      drive_.acceptInput(0, 0, 0);
      currentState = STATES::RETURN;
    }
    else
    {
      drive_.acceptInput(60, 0, 0);
    }
    break;

  case STATES::RETURN:
    Serial.println("Estado: RETURN");
    drive_.acceptInput(0, 0, 180);
    if (distance_sensor_.isObstacle())
    {
      currentState = STATES::AVOID_OBSTACLE_LEFT_RETURN;
    }else{
      drive_.acceptInput(0, 70, 0);
    }

    if(!distance_sensor_.obstacleInThePath())
    {
      currentState = STATES::GO_BEGINNING;
    }
    break;

  case STATES::AVOID_OBSTACLE_LEFT_RETURN:
    Serial.println("AVOID OBSTACLE LEFT RETURN STATE");
    bluetooth.println("AVOID OBSTACLE LEFT RETURN STATE");
    if (line_sensor_.isLeftLine())
    {
      start_time = 0;
      currentState = STATES::AVOID_OBSTACLE_RIGHT_RETURN;
    }
    else
    {
      drive_.acceptInput(-60, 0, 180);
    }

    if (start_time == 0)
    {
      start_time = millis();
    }

    if (!distance_sensor_.isObstacle() && millis() - start_time > 5000)
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
    if (line_sensor_.isRightLine())
    {
      start_time = 0;
      currentState = STATES::AVOID_OBSTACLE_LEFT_RETURN;
    }
    else
    {
      drive_.acceptInput(60, 0, 180);
    }

    if (start_time == 0)
    {
      start_time = millis();
    }

    if (!distance_sensor_.isObstacle() && millis() - start_time > 5000)
    {
      start_time = 0;
      drive_.acceptInput(0, 0, 180);
      drive_.hardBrake();
      currentState = STATES::GO_BEGINNING;
    }
    break;

  case STATES::GO_BEGINNING:
    Serial.println("GO BEGINNING STATE");
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
}