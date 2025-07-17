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
#include "robot/robot_instances.h"

//StateManager state_manager;

const unsigned long UPDATE_INTERVAL = 50;

double state_start_time = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  //state_manager.setState(RobotState::INIT);
  
  interrupts();
}


void loop()
{
  //state_manager.update();
  //drive_.update();

  drive_.moveForward(100);
  delay(10000);
  drive_.moveBackward(100);
  delay(10000);
  drive_.moveLeft(100);
  delay(10000);
  drive_.moveRight(100);
  delay(10000);

  delay(UPDATE_INTERVAL);
}

