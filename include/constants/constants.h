/**
 * @file constants.h
 * @date 24/04/2025
 * @author Juan Pablo Gutiérrez
 *
 * @brief Constants for the robot.
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>

namespace Constants
{

    namespace SystemConstants
    {
        constexpr float kUpdateInterval = 20.0;
    }

    // Units: centimeters
    namespace DistanceSensorConstants
    {
        // Distance threshold to consider an obstacle detected
        constexpr int kObstacleDistance = 40;

        // Target distance from the pool from which to move horizontally
        constexpr float kPoolTargetDistance = 20.0;

        constexpr float kApproachDistanceTargetControllerKp = 2.5;
        constexpr float kApproachDistanceTargetControllerKi = 0.00;
        constexpr float kApproachDistanceTargetControllerKd = 0.00;

        constexpr float kRetreatDistanceTargetControllerKp = 2.0;
        constexpr float kRetreatDistanceTargetControllerKi = 0.00;
        constexpr float kRetreatDistanceTargetControllerKd = 0.00;
    }

    // Units: meters, seconds, radians
    namespace DriveConstants
    {
        constexpr float kWheelDiameter = 0.1;
        constexpr float kWheelRadius = kWheelDiameter / 2.0;
        constexpr float kWheelCircumference = 2 * M_PI * kWheelRadius;

        constexpr float kHeadingControllerKp = 165.0f;
        constexpr float kHeadingControllerKi = 0.0f;
        constexpr float kHeadingControllerKd = 10.0f;
    }

    namespace IntakeConstants
    {
        constexpr int kGrabBallUpperServoPosition = 130;
        constexpr int kAvoidBallUpperServoPosition = 40;

        constexpr int kGrabBallLowerServoPosition = 95;
        constexpr int kAvoidBallLowerServoPosition = 140;

        constexpr int kIntakeRampPositioned = 130;
        constexpr int kIntakeRampStored = 40;
    }

    namespace BeanConstants
    {
        enum BeanType
        {
            NONE,
            MADURO,
            SOBREMADURO
        };
    }
}

#endif