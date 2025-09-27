#include <Arduino.h>
#include <Wire.h>
#include "../robot/robot_instances.h"

void setup()
{
  Serial.begin(9600);
  line_sensor_.begin();
}

void loop()
{
  line_sensor_.update();
  line_sensor_.printSensors();
  delay(100);
}