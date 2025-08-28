/**
 * LARC 2025
 *
 * @Roborregos
 */

#include <Arduino.h>
#include <Wire.h>

#include "robot/robot_instances.h"

bool line_crossed = false;
bool horizontal_line_found = false;
bool first_time = true;
bool kill = false;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  Wire.begin();

  // All systems must begin after initializing the serial and as the code starts running
  drive_.begin();
  elevator_.begin();
  com_.begin();

  drive_.setState(0),
      drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));
}

void loop()
{
  drive_.update();
  // line_sensor_.readSensor(Pins::kLineSensorFL);
  // line_sensor_.readSensor(Pins::kLineSensorFR);
  // delay(500);
  // delay(500);
  // line_sensor_.readSensor(Pins::kRightDistanceSensor);
  // Va para enfrente

  if (kill)
  {
    drive_.acceptInput(0, 0, 0);
    return;
  }

  if (!line_crossed)
  {
    drive_.acceptInput(0, 100, 0);
    if (line_sensor_.readSensor(Pins::kLineSensorFR) ||
        line_sensor_.readSensor(Pins::kLineSensorFL))
    {
      line_crossed = true;
    }
  }
  else
  {
    if (first_time)
    {
      drive_.acceptInput(0, 0, 0);
      Serial.println("STOPPED LINE");
      first_time = false;
      delay(400);
    }
    else
    {
      if (horizontal_line_found)
      {
        drive_.acceptInput(0, 0, 0);
        Serial.println("STOPPED HORIZONTAL");
        kill = true;
        delay(400);
      }
      else
      {
        drive_.acceptInput(200, 0, 0);
        Serial.println("MOVING");
      }

      if (line_sensor_.readSensor(Pins::kLineSensorFR))
      {
        horizontal_line_found = true;
      }
    }
  }
}