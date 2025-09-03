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
  line_sensor_.begin();
  distance_sensor_.begin();
  
  drive_.setState(0),
  drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));
}

void loop()
{
  // drive_.update();
  // delay(50);
  auto line_sensor_data = line_sensor_.readSensors();
  for (int i = 0; i < line_sensor_data.size(); i++)
  {
    Serial.print(line_sensor_data[i]);
    if (i < line_sensor_data.size() - 1)
    {
      Serial.print(" ");
    }
  }
  Serial.println();
  // distance_sensor_.getArrayDistance();
}
