/**
 * @file StateUtils.cpp
 * @date 30/09/2025
 * @author Juan Pablo Gutiérrez
 *
 * @brief Implementation of utility functions for the state machine.
 */

#include "StateUtils.h"

// Define the PID controllers
PIDController leftDistancePID(DistanceSensorConstants::kDistanceTargetControllerKp, DistanceSensorConstants::kDistanceTargetControllerKi, DistanceSensorConstants::kDistanceTargetControllerKd, -150.0, 150.0);
PIDController rightDistancePID(DistanceSensorConstants::kDistanceTargetControllerKp, DistanceSensorConstants::kDistanceTargetControllerKi, DistanceSensorConstants::kDistanceTargetControllerKd, -150.0, 150.0);

PIDController linePID(20.0, 0.0, 0.1, -100.0, 100.0);
PIDController rotationPID(8.0, 0.0, 0.2, -100.0, 100.0);


void maintainDistance(float distance, float lateralSpeed)
{
    float leftDistance = distance_sensor_.getDistance(0);
    float rightDistance = distance_sensor_.getDistance(1);

    float leftOutput = leftDistancePID.update(leftDistance, distance);
    float rightOutput = rightDistancePID.update(rightDistance, distance);

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

void followLine(float lateralSpeed)
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
  static const unsigned long BACKWARD_DURATION = 400;
  static unsigned long forwardStartTime = 0;
  static const unsigned long FORWARD_DURATION = 400;

  if (isLineDetected)
  {
    // If we were going backward and now detect the line, change direction to forward
    if (wasGoingBackward)
    {
      wasGoingBackward = false;
    }

    float vy_correction = linePID.update(positionError, 0.0);
    float omega_correction = rotationPID.update(rotationError, 0.0);

    drive_.acceptInput(lateralSpeed, vy_correction, omega_correction);

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
      drive_.acceptInput(lateralSpeed * 0.7, recovery_vy, 0.0);
    }
    else
    {
      // Continue going backward for the first 100ms
      float recovery_vy = (lastKnownPositionError > 0) ? 40.0 : -40.0;
      drive_.acceptInput(lateralSpeed * 0.7, recovery_vy, 0.0);
    }

    bluetooth.println("Searching line");
  }
}