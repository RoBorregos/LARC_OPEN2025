#include <Arduino.h>
#include <Wire.h>
#include "../robot/robot_instances.h"

void setup()
{
  Serial.begin(9600);
  distance_sensor_.begin();
}

void loop()
{
  distance_sensor_.update();
  Serial.print("Left distance: ");
  Serial.print(distance_sensor_.getDistance(0));
  Serial.print(" cm  Right distance: ");
  Serial.print(distance_sensor_.getDistance(1));
  Serial.println(" cm");
  delay(100);
}