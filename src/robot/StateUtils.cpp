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

PIDController linePID(10.0, 0.0, 0.2, -100.0, 100.0);
PIDController rotationPID(250.0, 0.0, 0.2, -100.0, 100.0);


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
  static bool wasGoingForward = false;
  static unsigned long backwardStartTime = 0;
  static const unsigned long BACKWARD_DURATION = 250;
  static unsigned long forwardStartTime = 0;
  static const unsigned long FORWARD_DURATION = 250;

  if (isLineDetected)
  {
    // If we were going backward/forward and now detect the line, reset states
    if (wasGoingBackward || wasGoingForward)
    {
      wasGoingBackward = false;
      wasGoingForward = false;
    }

    float vy_correction = linePID.update(positionError, 0.0);
    float omega_correction = rotationPID.update(rotationError, 0.0);

    drive_.acceptInput(lateralSpeed, vy_correction, omega_correction);

    lastKnownPositionError = positionError;
  }
  else
  {
    if (!wasGoingBackward && !wasGoingForward)
    {
      wasGoingBackward = true;
      backwardStartTime = millis();
    }

    // Check if backward duration has passed
    if (wasGoingBackward && millis() - backwardStartTime >= BACKWARD_DURATION)
    {
      // Switch to forward phase
      wasGoingBackward = false;
      wasGoingForward = true;
      forwardStartTime = millis();
    }

    if (wasGoingBackward)
    {
      // Continue going backward for the first 250ms
      float recovery_vy = (lastKnownPositionError > 0) ? 40.0 : -40.0;
      drive_.acceptInput(lateralSpeed * 0.7, recovery_vy, 0.0);
    }
    else if (wasGoingForward && millis() - forwardStartTime < FORWARD_DURATION)
    {
      // Go forward for 250ms after backward phase
      float recovery_vy = (lastKnownPositionError > 0) ? -40.0 : 40.0;
      drive_.acceptInput(lateralSpeed * 0.7, recovery_vy, 0.0);
    }
    else
    {
      // Reset states to start over with backward phase
      wasGoingForward = false;
    }

    bluetooth.println("Searching line");
  }
}