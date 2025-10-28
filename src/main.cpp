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
  monitor_.begin();
  Wire.begin();

  // All systems must begin after initializing the serial and as the code starts running
  monitor_.println("Initializing systems...");
  drive_.begin();
  com_.begin();
  line_sensor_.begin();
  distance_sensor_.begin();
  // intake_.begin();
  stateMachine.begin();
  elevator_.begin();
  monitor_.println("All systems initialized...");
  sorter_.begin();
  intake_.begin();
  drive_.setState(0);

  delay(1000);
  // drive_.acceptHeadingInput(Rotation2D::fromDegrees(180.0f));

  // Wait for "r" message from Bluetooth before continuing
  // monitor_.println("Waiting for ready command (r)...");

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

  // drive_.acceptInput(-90, 0, 0);
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