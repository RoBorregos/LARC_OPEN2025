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
  static float estimatedError = 0.0f;

  auto lineValues = line_sensor_.readSensors();
  bool left = lineValues[0];
  bool center = lineValues[4];
  bool right = lineValues[1];

  // IMU data
  auto accel = drive_.getLinearAcceleration(); // (ax, ay, az)
  float aY_filtered = accelFilterY.update(std::get<1>(accel));

  float targetFrontOutput = 0.0f;

  if (center || left || right)
  {
    if (center)
      estimatedError = 0.0f;
    else
      estimatedError *= 0.3f; // keep a bit of residual for smooth transition

    if (center)
      targetFrontOutput = 0.0f;
    else if (left && !right)
      targetFrontOutput = -30.0f;
    else if (right && !left)
      targetFrontOutput = 30.0f;
    else
      targetFrontOutput = 0.0f;

    monitor_.println(targetFrontOutput);
  }
  else
  {
    // integrate approximate vertical velocity
    estimatedError += aY_filtered * dt;

    // decay slowly to avoid runaway drift
    estimatedError *= 0.995f;

    targetFrontOutput = estimatedError * 110.0f;
  }

  targetFrontOutput = std::clamp(targetFrontOutput, -80.0f, 80.0f);
  drive_.acceptInput(lateralSpeed, targetFrontOutput, 0.0f);
}

// void followLineHybrid(float lateralSpeed, float dt)
// {
//   static float estimatedError = 0.0;
//   static float lastAngle = 0.0;

//   auto lineValues = line_sensor_.readSensors();
//   bool left = lineValues[0];
//   bool center = lineValues[4];
//   bool right = lineValues[1];

//   // IMU data
//   float angle = drive_.getYaw();                                          // degrees
//   std::tuple<float, float, float> accel = drive_.getLinearAcceleration(); // m/s^2
//   float aY_filtered = accelFilterY.update(std::get<1>(accel));
//   float aX_filtered = accelFilterX.update(std::get<0>(accel));

//   float targetFrontOutput = 0.0;

//   if (center || left || right)
//   {
//     estimatedError = 0.0;
//     lastAngle = angle;

//     if (center)
//       targetFrontOutput = 0.0;
//     else if (left && !right)
//       targetFrontOutput = -35.0;
//     else if (right && !left)
//       targetFrontOutput = 35.0;

//     monitor_.print("Line detected | ");
//     monitor_.print(targetFrontOutput);
//     monitor_.println();
//   }
//   else
//   {
//     // --- Inertial recovery ---
//     float headingRad = angle * M_PI / 180.0f;
//     float lateralAccel = aX_filtered * cosf(headingRad) - aY_filtered * sinf(headingRad);
//     estimatedError += lateralAccel * dt * 2.5f; // integrate lateral velocity

//     targetFrontOutput = estimatedError * 100.0f; // big enough gain to feel it
//     monitor_.print("No line | EstErr: ");
//     monitor_.print(estimatedError);
//     monitor_.print(" | aX: ");
//     monitor_.print(aX_filtered);
//     monitor_.print(" | aY: ");
//     monitor_.print(aY_filtered);
//     monitor_.print(" | Target: ");
//     monitor_.print(targetFrontOutput);
//     monitor_.println();
//   }

//   // Clamp to avoid excessive command
//   targetFrontOutput = std::clamp(targetFrontOutput, -80.0f, 80.0f);

//   drive_.acceptInput(lateralSpeed, targetFrontOutput, 0.0);
// }

void followLine(float lateralSpeed)
{
  auto lineValues = line_sensor_.readSensors();
  bool frontLeft = lineValues[0];
  bool frontRight = lineValues[1];
  bool frontCenter = lineValues[4];

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
