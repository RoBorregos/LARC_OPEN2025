/**
 * LARC 2025
 *
 * @Roborregos
 */

#include <Arduino.h>
#include <Wire.h>

#include "robot/robot_instances.h"
#include "robot/StateMachine.hpp"
#include <SoftwareSerial.h>

// --- Bluetooth Setup ---
SoftwareSerial bluetooth(0, 1); // RX, TX pins for Bluetooth module

// State machine instance
StateMachine stateMachine(bluetooth);

void setup()
{
  Serial.begin(9800);
  bluetooth.begin(9800);
  Serial.println("Starting...");
  Wire.begin();

  // All systems must begin after initializing the serial and as the code starts running
  drive_.begin();
  com_.begin();
  line_sensor_.begin();
  distance_sensor_.begin();
  drive_.setState(0);
  drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));
  
  // Initialize state machine
  stateMachine.begin();
}

void loop()
{
  drive_.update();
  stateMachine.update();
}