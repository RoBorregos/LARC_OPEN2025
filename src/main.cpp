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
  START, // Represents the start of the robot
  AVOID_OBSTACLE_LEFT,
  AVOID_OBSTACLE_RIGHT,
  GO_STRAIGHT,
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

  // auto distances = distance_sensor_.getArrayDistance();
  // float frontLeft = distances[0];
  // float frontRight = distances[1];

  // Serial.print("Front Left Distance: ");
  // Serial.print(frontLeft);
  // Serial.print(" cm, Front Right Distance: ");
  // Serial.print(frontRight);
  // Serial.println(" cm");

  switch (currentState)
  {
  case STATES::START:
    Serial.println("START STATE");
    if (distance_sensor_.isObstacle())
    {
      drive_.acceptInput(0, 0, 0);
      currentState = STATES::AVOID_OBSTACLE_LEFT;
    }
    else
    {
      drive_.acceptInput(0, 70, 0);
      if (start_time == 0)
      {
        start_time = millis();
      }
      if (!distance_sensor_.isObstacle())
      {
        currentState = STATES::GO_STRAIGHT;
      }
    }
    break;

  case STATES::AVOID_OBSTACLE_LEFT:
    Serial.println("AVOID OBSTACLE LEFT STATE");

    if (line_sensor_.isLeftLine())
    {
      currentState = STATES::AVOID_OBSTACLE_RIGHT;
    }
    else
    {
      drive_.acceptInput(-60, 0, 0);
    }

    if (!distance_sensor_.isObstacle())
    {
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();
      currentState = STATES::GO_STRAIGHT;
    }
    break;

  case STATES::AVOID_OBSTACLE_RIGHT:
    Serial.println("AVOID OBSTACLE RIGHT STATE");
    if (line_sensor_.isRightLine())
    {
      currentState = STATES::AVOID_OBSTACLE_LEFT;
    }
    else
    {
      drive_.acceptInput(60, 0, 0);
    }

    if (!distance_sensor_.isObstacle())
    {
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();
      currentState = STATES::GO_STRAIGHT;
    }
    break;

  case STATES::GO_STRAIGHT:
    Serial.println("GO_STRAIGHT STATE");
    if (line_sensor_.isFrontLine())
    {
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();
      currentState = STATES::ENDLINE;
    }
    else
    {
      drive_.acceptInput(0, 70, 0);
    }

    break;

  case STATES::ENDLINE:
    drive_.acceptInput(0, 0, 0);
    // Serial.println("ENDLINE STATE");
    // if (line_sensor_.isLeftLine() && line_sensor_.isFrontLine())
    // {
    //   drive_.acceptInput(0, 0, 0);
    //   currentState = STATES::RIGHTMOST; // esto esta mals
    // }
    // else
    // {
    //   // drive_.followFrontLine(0);
    // }
    break;

  case STATES::RIGHTMOST:
    drive_.acceptInput(0, 0, 0);
    // Serial.println("Estado: RIGHTMOST");
    // if (line_sensor_.isRightLine())
    // {
    //   drive_.acceptInput(0, 0, 0);
    //   currentState = STATES::RETURN;
    // }
    // else
    // {
    //   drive_.followFrontLine(1);
    // }
    break;

  case STATES::RETURN:
    drive_.acceptInput(0, 0, 0);
    break;
    // Serial.println("Estado: RETURN");
    // drive_.acceptInput(0, 0, 180);
    // if (distance_sensor_.isObstacle())
    // {
    //   if (start_time == 0)
    //   {
    //     start_time = millis();
    //   }
    //   if (millis() - start_time > 500)
    //   {
    //     if (line_sensor_.isLeftLine())
    //     {
    //       drive_.acceptInput(100, 0, 0);
    //     }
    //     else
    //     {
    //       drive_.acceptInput(-100, 0, 0);
    //     }
    //   }
    // }
    // else
    // {
    //   drive_.acceptInput(0, 50, 0);
    //   if (line_sensor_.isFrontLine() && (millis() - start_time > 3000))
    //   {
    //     drive_.acceptInput(0, 0, 0);
    //     currentState = STATES::START;
    //   }
    // }
    break;

  default:
    Serial.println("Error: Estado desconocido.");
    drive_.acceptInput(0, 0, 0);
    break;
  }
}