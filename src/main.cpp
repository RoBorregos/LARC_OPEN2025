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

// --- Bluetooth Setup ---
SoftwareSerial bluetooth(0, 1); // RX, TX pins for Bluetooth module

// State machine instance
StateMachine stateMachine(bluetooth);

// PID controller for line following using front sensors
PIDController linePID(20.0, 0.0, 0.1, -100.0, 100.0);
PIDController rotationPID(8.0, 0.0, 0.2, -100.0, 100.0);

// Line following parameters
const float LATERAL_SPEED = 70.0;

PIDController leftDistancePID(DistanceSensorConstants::kDistanceTargetControllerKp, DistanceSensorConstants::kDistanceTargetControllerKi, DistanceSensorConstants::kDistanceTargetControllerKd, -150.0, 150.0);  // kp, ki, kd, min, max
PIDController rightDistancePID(DistanceSensorConstants::kDistanceTargetControllerKp, DistanceSensorConstants::kDistanceTargetControllerKi, DistanceSensorConstants::kDistanceTargetControllerKd, -150.0, 150.0); // kp, ki, kd, min, max

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
  String btInput = "";
  bluetooth.println("Waiting for ready command (r)...");

  while (true)
  {
    if (bluetooth.available())
    {
      char c = bluetooth.read();
      if (c == '\n' || c == '\r')
      {
        btInput.trim();
        if (btInput.equalsIgnoreCase("r"))
        {
          Serial.println("Bluetooth ready received.");
          break;
        }
        btInput = "";
      }
      else
      {
        btInput += c;
      }
    }
  }
}

void loop()
{
  // Update drive system
  drive_.update();

  delay(20);
}

void followLine()
{
  // Get current line senssor readings
  auto lineValues = line_sensor_.readSensors();
  bool frontLeft = lineValues[0];
  bool frontRight = lineValues[1];

  float frontError = 0.0;
  if (frontLeft && !frontRight)
    frontError = -1.0;
  if (!frontLeft && frontRight)
    frontError = 1.0;

  float positionError = frontError;

  float rotationError = frontError;

  bool isLineDetected = frontLeft || frontRight;
  static float lastKnownPositionError = 0.0;
  static bool wasGoingBackward = false;
  static unsigned long backwardStartTime = 0;
  static const unsigned long BACKWARD_DURATION = 200;
  static unsigned long forwardStartTime = 0;
  static const unsigned long FORWARD_DURATION = 100;

  if (isLineDetected)
  {
    // If we were going backward and now detect the line, change direction to forward
    if (wasGoingBackward)
    {
      wasGoingBackward = false;
    }

    float vy_correction = linePID.update(positionError, 0.0);
    float omega_correction = rotationPID.update(rotationError, 0.0);

    drive_.acceptInput(LATERAL_SPEED, vy_correction, omega_correction);

    lastKnownPositionError = positionError;
  }
  else
  {
    if (!wasGoingBackward)
    {
      wasGoingBackward = true;
      backwardStartTime = millis();
    }

    // Check if 100ms have passed since going backward
    if (millis() - backwardStartTime >= BACKWARD_DURATION)
    {
      // Change direction to forward after 100ms
      float recovery_vy = (lastKnownPositionError > 0) ? -40.0 : 40.0;
      drive_.acceptInput(LATERAL_SPEED * 0.7, recovery_vy, 0.0);
    }
    else
    {
      // Continue going backward for the first 100ms
      float recovery_vy = (lastKnownPositionError > 0) ? 40.0 : -40.0;
      drive_.acceptInput(LATERAL_SPEED * 0.7, recovery_vy, 0.0);
    }

    bluetooth.println("Searching line");
  }
}

void maintainDistance(float distance, float lateralSpeed)
{
  float leftDistance = distance_sensor_.getDistance(0);
  float rightDistance = distance_sensor_.getDistance(1);

  float leftOutput = leftDistancePID.update(leftDistance, DistanceSensorConstants::kPoolTargetDistance);
  float rightOutput = rightDistancePID.update(rightDistance, DistanceSensorConstants::kPoolTargetDistance);

  float forwardOutput = (leftOutput + rightOutput) / 2.0 * -1;

  drive_.acceptInput(lateralSpeed, forwardOutput, 0.0);

  bluetooth.print("L: ");
  bluetooth.print(leftDistance);
  bluetooth.print("cm (");
  bluetooth.print(leftOutput);
  bluetooth.print(") R: ");
  bluetooth.print(rightDistance);
  bluetooth.print("cm (");
  bluetooth.print(rightOutput);
  bluetooth.print(") Fwd: ");
  bluetooth.print(forwardOutput);
}