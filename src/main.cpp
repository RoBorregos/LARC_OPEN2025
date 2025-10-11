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
#include "../../lib/controllers/PIDController.hpp"
#include "constants/constants.h"

using namespace Constants;

// State machine instance
StateMachine stateMachine(bluetooth);

void setup()
{
  Serial.begin(9800);
  bluetooth.begin(9800);
  bluetooth.println("Starting...");
  Wire.begin();

  // All systems must begin after initializing the serial and as the code starts running
  bluetooth.println("Initializing systems...");
  drive_.begin();
  com_.begin();
  line_sensor_.begin();
  distance_sensor_.begin();
  stateMachine.begin();
  bluetooth.println("All systems initialized...");

  drive_.setState(0);
  drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));

  // Wait for "r" message from Bluetooth before continuing
  bluetooth.println("Waiting for ready command (r)...");
  
  String btInput = "";
  // while (true)
  // {
  //   if (bluetooth.available())
  //   {
  //     char c = bluetooth.read();
  //     if (c == '\n' || c == '\r')
  //     {
  //       btInput.trim();
  //       if (btInput.equalsIgnoreCase("r"))
  //       {
  //         Serial.println("Bluetooth ready received.");
  //         break;
  //       }
  //       btInput = "";
  //     }
  //     else
  //     {
  //       btInput += c;
  //     }
  //   }
  // }
}

void loop()
{
  // stateMachine.update();

  line_sensor_.printSensors();

  // delay(20);
}
