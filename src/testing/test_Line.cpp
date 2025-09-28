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
  Serial.print(line_sensor_.isFrontLine() ? "front yes" : "front no");
  Serial.print(",");
  Serial.print(line_sensor_.isBackLine() ? "back yes" : "back no");
  Serial.print(",");
  Serial.print(line_sensor_.isLeftLine() ? "left yes" : "left no");
  Serial.print(",");
  Serial.println(line_sensor_.isRightLine() ? "right yes" : "right no");
  delay(100);
}