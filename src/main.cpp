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
StateMachine stateMachine(monitor_);

void setup()
{
  monitor_.begin();
  Wire.begin();

  // All systems must begin after initializing the serial and as the code starts running
  monitor_.println("Initializing systems...");
  drive_.begin();
  com_.begin();
  line_sensor_.begin();
  distance_sensor_.begin();
  intake_.begin();
  stateMachine.begin();
  monitor_.println("All systems initialized...");

  drive_.setState(0);
  drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));

  // Wait for "r" message from Bluetooth before continuing
  monitor_.println("Waiting for ready command (r)...");

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
  drive_.update();

  stateMachine.update();

  // maintainDistance(DistanceSensorConstants::kPoolTargetDistance, 0);
  // maintainDistance(DistanceSensorConstants::kPoolTargetDistance, 110);

  delay(SystemConstants::kUpdateInterval);
}
