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
    Serial.print("Too close: ");
    // If we are too close, we need to back up
    leftOutput = retreatLeftDistancePID.update(leftDistance, distance) * -1;
    rightOutput = retreatRightDistancePID.update(rightDistance, distance) * -1;
  }
  else
  {
    Serial.print("Too far: ");
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
  }

  Serial.print("L: ");
  Serial.print(leftDistance);
  Serial.print("cm (");
  Serial.print(leftOutput);
  Serial.print(") R: ");
  Serial.print(rightDistance);
  Serial.print("cm (");
  Serial.print(rightOutput);
  Serial.print(") Fwd: ");
  Serial.print(forwardOutput);
  Serial.print(" Error: ");
  Serial.print(error.getDegrees());

  Serial.println();
}

struct LowPassFilter
{
  float alpha;
  float value;

  LowPassFilter(float alpha = 0.2f) : alpha(alpha), value(0.0f) {}

  float update(float input)
  {
    value += alpha * (input - value);
    return value;
  }
};

LowPassFilter accelFilterX(0.15f); // 0.05–0.2 typical range
LowPassFilter accelFilterY(0.15f); // 0.05–0.2 typical range

void followLineHybrid(float lateralSpeed, float dt)
{
  static float estimatedPositionError = 0.0f; // IMU-based lateral offset estimate (meters)
  static float accelBiasY = 0.0f;             // Slow bias tracker for sideways acceleration drift
  static bool lastLineOnRight = false;        // Remember which edge saw the line last

  dt = std::max(0.0f, dt);  // Reject negative sampling intervals
  dt = std::min(dt, 0.05f); // Cap dt to avoid huge integration jumps

  auto lineValues = line_sensor_.readSensors();
  bool left = lineValues[0];
  bool right = lineValues[1];
  bool center = lineValues[4];

  auto accel = drive_.getLinearAcceleration();              // Raw acceleration in robot body frame (m/s²)
  float yawRad = drive_.getYaw() * M_PI / 180.0f;           // Robot heading in radians
  float a_body_y = accelFilterY.update(std::get<1>(accel)); // Filtered forward-axis accel (robot frame)
  float a_world_sideways = cosf(yawRad) * a_body_y;         // Rotate to field sideways axis

  float targetFrontOutput = 0.0f; // Command along robot front/back (controls recentering)

  if (center || left || right)
  {
    accelBiasY += 0.05f * (a_world_sideways - accelBiasY); // Update bias while sensors are trusted
    estimatedPositionError = 0.0f;                         // Reset IMU integral when line is visible
    lastLineOnRight = right && !left;                      // Record which side had the line
    if (center)
      lastLineOnRight = false; // If center, bias to left

    targetFrontOutput = center ? 0.0f : (left ? -30.0f : 30.0f); // Sensor-based correction
  }
  else
  {
    float correctedAccel = a_world_sideways - accelBiasY;                     // Remove bias to reduce drift
    estimatedPositionError += correctedAccel * dt;                            // Integrate sideways motion
    estimatedPositionError = std::clamp(estimatedPositionError, -0.6f, 0.6f); // Limit estimate

    float gain = 130.0f;                                   // Convert position error to command
    if (estimatedPositionError == 0.0f && lastLineOnRight) // If ambiguous, bias toward last seen edge
      estimatedPositionError = 0.05f;

    targetFrontOutput = std::clamp(estimatedPositionError * gain, -80.0f, 80.0f); // Clamp output
  }

  Serial.print("Line L:");
  Serial.print(left);
  Serial.print(" R:");
  Serial.print(right);
  Serial.print(" C:");
  Serial.print(center);
  Serial.print(" | a_y:");
  Serial.print(a_world_sideways, 3);
  Serial.print(" estErr:");
  Serial.print(estimatedPositionError, 3);
  Serial.print(" tgtF:");
  Serial.println(targetFrontOutput, 1);
  Serial.println();

  drive_.acceptInput(lateralSpeed, targetFrontOutput, 0.0f); // Apply lateral speed + correction
}
