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
  Wire.begin();

  drive_.moveForward(0);
}

void loop()
{
}
