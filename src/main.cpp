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
}

void loop()
{
  drive_.motorTest();
  // drive_.acceptInput(0,200,0);
  // delay(2000);
  // drive_.acceptInput(200,0,0);
  // delay(2000);
}
