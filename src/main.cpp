/**
 * LARC 2025
 *
 * @Roborregos
 */

#include <Arduino.h>
#include <Wire.h>
#include "pose2d.hpp"
#include "robot/statemanager.hpp"
#include "subsystem/Drive/Drive.hpp"

StateManager state_manager;

const unsigned long UPDATE_INTERVAL = 50;

double state_start_time = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  state_manager.setState(RobotState::PICK_LOW_LEVEL);
  interrupts();
}

void loop()
{
  state_manager.update();

  delay(UPDATE_INTERVAL);
}
