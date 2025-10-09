#include <Arduino.h>
#include <Wire.h>
#include "../robot/robot_instances.h"

void setup()
{
  Serial.begin(9600);
  drive_.begin();
  line_sensor_.begin();
}

void loop()
{
  drive_.update();
  drive_.moveForward(80);
  if (line_sensor_.isFrontLine())
  {
    Serial.println("LINE DETECTED");
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    delay(1000);
  }
  // delay(2000);
  // drive_.brake();
  // drive_.moveBackward(100);
  // delay(2000);
  // drive_.brake();
  // drive_.moveLeft(100);
  // delay(2000);
  // drive_.brake();
  // drive_.moveRight(100);
  // delay(2000);  
}