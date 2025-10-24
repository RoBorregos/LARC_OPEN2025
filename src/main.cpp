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
  stateMachine.begin();
  monitor_.println("All systems initialized...");

  drive_.setState(0);
  drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));

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
}

void loop()
{
  // bool detected = line_sensor_.isLeftLine();

  // if (detected)
  // {
  //   monitor_.println("Line detected on the left side!");
  // }
  // else
  // {
  //   monitor_.println("No line detected on the left side.");
  // }

  drive_.update();

  // monitor_.print("Left distance: ");

  // monitor_.print(distanceLeft);
  // monitor_.print(" cm  Valid: ");
  // monitor_.print(validLeft ? "Yes " : "No ");
  // monitor_.print(" cm  Right distance: ");
  // monitor_.print(distanceRight);
  // monitor_.print(" cm  Valid: ");
  // monitor_.print(validRight ? "Yes " : "No ");

  // auto [isObstacle, obstacleValid] = distance_sensor_.isObstacle();
  // monitor_.print(isObstacle ? "Obstacle yes" : "Obstacle no");
  // monitor_.println();
  // drive_.acceptInput(0, 0.8, 0);
  distance_sensor_.update();
  stateMachine.update();

  // auto [distanceLeft, validLeft] = distance_sensor_.getDistance(0);
  // auto [distanceRight, validRight] = distance_sensor_.getDistance(1);
  // Serial.print("Left distance: ");

  // Serial.print(distanceLeft);
  // Serial.print(" cm  Valid: ");
  // Serial.print(validLeft ? "Yes " : "No ");
  // Serial.print(" cm  Right distance: ");
  // Serial.print(distanceRight);
  // Serial.print(" cm  Valid: ");
  // Serial.print(validRight ? "Yes " : "No ");

  // auto [isObstacle, obstacleValid] = distance_sensor_.isObstacle();
  // Serial.print(isObstacle ? "Obstacle yes" : "Obstacle no");
  // Serial.println();

  // drive_.acceptInput(0.5, 0, 0);
  // followLine(0);

  // sensors_event_t linearAccelData;
  // bno_.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);

  // monitor_.print("Linear Accel X: ");
  // monitor_.print(linearAccelData.acceleration.x);
  // monitor_.print(" Y: ");
  // monitor_.print(linearAccelData.acceleration.y);
  // monitor_.print(" Z: ");
  // monitor_.println(linearAccelData.acceleration.z);
  // stateMachine.update();

  // maintainDistance(DistanceSensorConstants::kPoolTargetDistance, 0);
  // maintainDistance(DistanceSensorConstants::kPoolTargetDistance, 110);

  delay(20);
}
