/**
 * @file StateUtils.cpp
 * @date 30/09/2025
 * @author Juan Pablo Gutiérrez
 *
 * @brief Implementation of utility functions for the state machine.
 */

#include "StateUtils.h"

// Define the PID controllers
PIDController approachLeftDistancePID(DistanceSensorConstants::kApproachDistanceTargetControllerKp, DistanceSensorConstants::kApproachDistanceTargetControllerKi, DistanceSensorConstants::kApproachDistanceTargetControllerKd, -150.0, 150.0);
PIDController approachRightDistancePID(DistanceSensorConstants::kApproachDistanceTargetControllerKp, DistanceSensorConstants::kApproachDistanceTargetControllerKi, DistanceSensorConstants::kApproachDistanceTargetControllerKd, -150.0, 150.0);

PIDController retreatLeftDistancePID(DistanceSensorConstants::kRetreatDistanceTargetControllerKp, DistanceSensorConstants::kRetreatDistanceTargetControllerKi, DistanceSensorConstants::kRetreatDistanceTargetControllerKd, -150.0, 150.0);
PIDController retreatRightDistancePID(DistanceSensorConstants::kRetreatDistanceTargetControllerKp, DistanceSensorConstants::kRetreatDistanceTargetControllerKi, DistanceSensorConstants::kRetreatDistanceTargetControllerKd, -150.0, 150.0);

PIDController lateralPID(20.0, 1.0, 2.0, -100.0, 100.0);

PIDController followLinePID(20.0, 0.0, 1.0, -80.0, 80.0);

void maintainDistance(float distance, float lateralSpeed)
{
  auto [leftDistance, leftValid] = distance_sensor_.getDistance(0);
  auto [rightDistance, rightValid] = distance_sensor_.getDistance(1);

  float currentDistance = (leftDistance + rightDistance) / 2.0;

  float leftOutput;
  float rightOutput;

  if (currentDistance < distance)
  {
    monitor_.print("Too close: ");
    // If we are too close, we need to back up
    leftOutput = retreatLeftDistancePID.update(leftDistance, distance) * -1;
    rightOutput = retreatRightDistancePID.update(rightDistance, distance) * -1;
  }
  else
  {
    monitor_.print("Too far: ");
    // If we are too far, we need to approach
    leftOutput = approachLeftDistancePID.update(leftDistance, distance) * -1;
    rightOutput = approachRightDistancePID.update(rightDistance, distance) * -1;
  }

  float forwardOutput = (leftOutput + rightOutput) / 2.0;

  Rotation2D error = drive_.getHeadingError();

  // Only correct if theading error is within 30 degrees
  if (std::abs(error.getDegrees()) < 30)
  {
    // if the difference between both sensors is less than 10, and both their measurements are valid, then correct
    if (std::abs(leftDistance - rightDistance) < 10 && leftValid && rightValid)
    {
      drive_.acceptInput(lateralSpeed, forwardOutput, 0.0);
      // else, move backwards a little to allow for heading correction
    }
    else
    {
      drive_.acceptInput(0, -50, 0);
    }
  }

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
  monitor_.print(" Error: ");
  monitor_.print(error.getDegrees());

  monitor_.println();
}

void followLineJp(float lateralSpeed)
{
  static int lastSeen = 0;        // -1 = behind, 1 = ahead, 0 = unknown
  static float frontOutput = 0.0; // smoothed command
  static int lostCounter = 0;     // how many cycles since last line detection

  auto lineValues = line_sensor_.readSensors();
  bool frontLeft = lineValues[0];
  bool frontRight = lineValues[1];

  float targetFrontOutput = 0.0;

  if (frontLeft && !frontRight)
  {
    targetFrontOutput = -30.0;
    lastSeen = -1;
    lostCounter = 0;
  }
  else if (!frontLeft && frontRight)
  {
    targetFrontOutput = 50.0;
    lastSeen = 1;
    lostCounter = 0;
  }
  else if (frontLeft && frontRight)
  {
    targetFrontOutput = 0.0; // centered
    lostCounter = 0;
  }
  else
  {
    // No detection
    lostCounter++;
    if (lostCounter < 25)
    { // < 500 ms at delay(20)
      // try to recover based on last seen
      if (lastSeen == -1)
        targetFrontOutput = -15.0;
      else if (lastSeen == 1)
        targetFrontOutput = 25.0;
      else
        targetFrontOutput = 0.0;
    }
    else
    {
      // Lost for too long → stop and reset
      targetFrontOutput = 0.0;
      lastSeen = 0;
    }
  }

  // Smooth transitions
  const float alpha = 0.2;
  frontOutput += alpha * (targetFrontOutput - frontOutput);

  drive_.acceptInput(lateralSpeed, frontOutput, 0.0);
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
