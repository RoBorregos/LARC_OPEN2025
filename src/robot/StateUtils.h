#pragma once

#include "../subsystem/DistanceSensors/DistanceSensor.hpp"
#include "constants/constants.h"
#include "../../lib/controllers/PIDController.hpp"
#include "robot/robot_instances.h"

using namespace Constants;

PIDController leftDistancePID(DistanceSensorConstants::kDistanceTargetControllerKp, DistanceSensorConstants::kDistanceTargetControllerKi, DistanceSensorConstants::kDistanceTargetControllerKd, -150.0, 150.0);  // kp, ki, kd, min, max
PIDController rightDistancePID(DistanceSensorConstants::kDistanceTargetControllerKp, DistanceSensorConstants::kDistanceTargetControllerKi, DistanceSensorConstants::kDistanceTargetControllerKd, -150.0, 150.0); // kp, ki, kd, min, max

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