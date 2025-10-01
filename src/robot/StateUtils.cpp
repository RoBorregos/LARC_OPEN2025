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

PIDController linePID(40.0, 0.0, 0.1, -100.0, 100.0);
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

void followLine(float LATERAL_SPEED)
{
  // Leer sensores
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

  // States
  static bool wasGoingBackward = false;
  static bool wasGoingForward = false;
  static float lastKnownPositionError = 0.0;
  static unsigned long backwardStartTime = 0;
  static unsigned long forwardStartTime = 0;

  // Constants
  static const unsigned long BACKWARD_DURATION = 400; // ms
  static const unsigned long FORWARD_DURATION  = 400; // ms
  static const float MAX_SEARCH_DISTANCE = 10;     // 10 cm sin línea
  static const unsigned long MAX_SEARCH_TIME = 1500;  // 1.5s sin línea

  // Acumuladores
  static float searchDistanceAccum = 0.0;
  static unsigned long lineLostTime = 0;

  if (isLineDetected)
  {
    wasGoingBackward = false;
    wasGoingForward  = false;
    searchDistanceAccum = 0.0;
    lineLostTime = millis();

    float vy_correction = linePID.update(positionError, 0.0);
    float omega_correction = rotationPID.update(rotationError, 0.0);

    drive_.acceptInput(LATERAL_SPEED, vy_correction, omega_correction);
    lastKnownPositionError = positionError;
  }
  else
  {
    if (lineLostTime == 0) {
      lineLostTime = millis();
      searchDistanceAccum = 0.0;
      drive_.resetEncoders();
    }

    searchDistanceAccum += drive_.getAverageDistanceTraveled();
    drive_.resetEncoders();

    if (searchDistanceAccum > MAX_SEARCH_DISTANCE || millis() - lineLostTime > MAX_SEARCH_TIME)
    {
      drive_.brake();
      bluetooth.println("Line lost");
      return;
    }

    if (!wasGoingBackward && !wasGoingForward) {
      wasGoingBackward = true;
      backwardStartTime = millis();
    }

    if (wasGoingBackward) {
      if (millis() - backwardStartTime >= BACKWARD_DURATION) {
        wasGoingBackward = false;
        wasGoingForward = true;
        forwardStartTime = millis();
      } else {
        float recovery_vy = (lastKnownPositionError > 0) ? 70.0 : -70.0;
        drive_.acceptInput(LATERAL_SPEED * 0.7, recovery_vy, 0.0);
      }
    }
    else if (wasGoingForward) {
      if (millis() - forwardStartTime >= FORWARD_DURATION) {
        wasGoingForward = false;
        wasGoingBackward = true;
        backwardStartTime = millis();
      } else {
        float recovery_vy = (lastKnownPositionError > 0) ? -70.0 : 70.0;
        drive_.acceptInput(LATERAL_SPEED * 0.7, recovery_vy, 0.0);
      }
    }

    bluetooth.println("Searching line...");
  }
}