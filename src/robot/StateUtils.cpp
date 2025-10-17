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

PIDController lateralPID(20.0, 1.0, 2.0, -100.0, 100.0);

PIDController followLinePID(20.0, 0.0, 1.0, -80.0, 80.0);

void maintainDistance(float distance, float lateralSpeed)
{
    float leftDistance = distance_sensor_.getDistance(0);
    float rightDistance = distance_sensor_.getDistance(1);

    float leftOutput = leftDistancePID.update(leftDistance, distance);
    float rightOutput = rightDistancePID.update(rightDistance, distance);

    float forwardOutput = (leftOutput + rightOutput) / 2.0 * -1;

    drive_.acceptInput(lateralSpeed, forwardOutput, 0.0);

    monitor_.print("L: ");
    monitor_.print(leftDistance);
    monitor_.print("cm (");
    monitor_.print(leftOutput);
    monitor_.print(") R: ");
    monitor_.print(rightDistance);
    monitor_.print("cm (");
    monitor_.print(rightOutput);
    monitor_.print(") Fwd: ");
    monitor_.print(forwardOutput);
}

void followLine(float lateralSpeed)
{
  auto lineValues = line_sensor_.readSensors();
  bool frontLeft = lineValues[0];
  bool frontRight = lineValues[1];

  float frontError = 0.0;
  if (frontLeft && !frontRight)
    frontError = -1.0;
  else if (!frontLeft && frontRight)
    frontError = 1.0;

  bool isLineDetected = frontLeft || frontRight;

  static float lastKnownError = 0.0;
  static float lastVyCorrection = 0.0;
  static unsigned long lastDetectionTime = 0;

  float vy_correction = 0.0;

  if (isLineDetected)
  {
    vy_correction = followLinePID.update(frontError, 0.0);
    lastKnownError = frontError;
    lastVyCorrection = vy_correction;
    lastDetectionTime = millis();
  }
  else if (millis() - lastDetectionTime < 150) // persistencia corta
  {
    // Mantén el último control brevemente
    vy_correction = lastVyCorrection;
  }
  else
  {
    // Busca lateralmente sin retroceder
    vy_correction = (lastKnownError > 0) ? 30.0 : -30.0;
  }

  drive_.acceptInput(lateralSpeed, vy_correction, 0.0);
}


void evadeLine(float lateralSpeed)
{
  auto lineValues = line_sensor_.readSensors();
  bool frontLeft = lineValues[0];
  bool frontRight = lineValues[1];
  bool isLineDetected = frontLeft || frontRight;

  float frontError = 0.0;
  if (frontLeft && !frontRight)
    frontError = -1.0;
  else if (!frontLeft && frontRight)
    frontError = 1.0;

  if (isLineDetected)
  {
    float backSpeed = -fabs(lateralSpeed) * 0.5;
    float vy_correction = lateralPID.update(-frontError, 0.0);

    drive_.acceptInput(backSpeed, vy_correction, 0.0);

    monitor_.print("Line detected | Error: ");
    monitor_.print(frontError);
    monitor_.print(" | vy_corr: ");
    monitor_.println(vy_correction);
  }
  else
  {
    drive_.acceptInput(lateralSpeed, 0.0, 0.0);
    monitor_.println("No line detected - moving lateral");
  }
}
