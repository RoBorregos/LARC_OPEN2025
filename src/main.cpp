/**
 * LARC 2025
 *
 * @Roborregos
 */

#include <Arduino.h>
#include <Wire.h>

#include "robot/robot_instances.h"
#include "robot/StateMachine.h"
#include "constants/constants.h"

using namespace Constants;

StateMachine stateMachine;

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  // All systems must begin after initializing the serial and as the code starts running
  Serial.println("Initializing systems...");
  drive_.begin();
  com_.begin();
  line_sensor_.begin();
  distance_sensor_.begin();
  stateMachine.begin();
  elevator_.begin();
  Serial.println("All systems initialized...");
  sorter_.begin();
  intake_.begin();
  drive_.setState(0);

  delay(1000);
}

void loop()
{
  drive_.update();
  com_.update();
  intake_.update();
  elevator_.update();
  sorter_.update();
  stateMachine.update();

  delay(SystemConstants::kUpdateInterval);
}