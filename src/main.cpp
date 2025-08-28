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

  if (!line_crossed)
  {
    drive_.acceptInput(0, 100, 0);
  }
  else
  {
    drive_.acceptInput(0, 0, 0);
    Serial.println("STOPPED");
  }

  if (line_sensor_.readSensor(Pins::kLineSensorFR) ||
      line_sensor_.readSensor(Pins::kLineSensorFL))
  {
    line_crossed = true;
    delay(400);
  }
}