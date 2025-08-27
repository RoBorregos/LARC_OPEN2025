/**
 * LARC 2025
 *
 * @Roborregos
 */

#include <Arduino.h>
#include <Wire.h>

#include "robot/robot_instances.h"

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
  // drive_.update();
  // delay(50);
  distance_sensor_.getArrayDistance();
}
