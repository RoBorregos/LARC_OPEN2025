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
        constexpr float kUpdateInterval = 50.0; // in ms -> 20Hz
    }

    namespace ElevatorConstants
    {
        constexpr float kUpperLevel = 0.0;
        constexpr float kMidLevel = 0.0;
        constexpr float kLowerLevel = 0.0;
        constexpr float kIdleLevel = 0.0;

        constexpr float kP = 0.01;
        constexpr float kI = 0.00;
        constexpr float kD = 0.00;

        constexpr bool kInverted = true;
        constexpr bool kEncoderActiveState = LOW;

    }

    namespace DistanceSensorConstants
    {
        constexpr int kObstacleDistance = 40;
        constexpr int kTreeDistance = 13;

        constexpr float kPoolTargetDistance = 20.0; // in cm
        constexpr float kTreeTargetDistance = 25.0; // in cm
        constexpr float kMaxTargetDistance = 80.0; // in cm
        constexpr float kApproachDistanceTargetControllerKp = 2.5;
        constexpr float kApproachDistanceTargetControllerKi = 0.00;
        constexpr float kApproachDistanceTargetControllerKd = 0.00; // ORIGINAL: 1.0

        constexpr float kRetreatDistanceTargetControllerKp = 2.0;
        constexpr float kRetreatDistanceTargetControllerKi = 0.00;
        constexpr float kRetreatDistanceTargetControllerKd = 0.00;
    }

    namespace LineSensorConstants
    {
        constexpr int kPoolDistance = 10;
        constexpr int kTreeDistance = 15;
    }

    namespace GripperConstants
    {
        constexpr int kOpenAngle = 140;
        constexpr int kClosedAngle = 40;
    }

    namespace UpperSorterConstants
    {
        constexpr int kLeftAngle = 0;
        constexpr int kCenterAngle = 90;
        constexpr int kRightAngle = 180;
    }

    namespace LowerSorterConstants
    {
        constexpr int kLeftAngle = 180;
        constexpr int kCenterAngle = 90;
        constexpr int kRightAngle = 0;
    }

    namespace DriveConstants
    {
        constexpr float kWheelDiameter = 0.1;                          // in m
        constexpr float kWheelRadius = kWheelDiameter / 2.0;           // in m
        constexpr float kWheelCircumference = 2 * M_PI * kWheelRadius; // in m

        constexpr float kHeadingControllerKp = 160.0f; // ORIGINAL: 240.0f - WORKING GREAT RN 28/09/2025 12:02 PM
        constexpr float kHeadingControllerKi = 0.0f;
        constexpr float kHeadingControllerKd = 10.0f;
    }

    namespace VisionConstants
    {
        constexpr float kCenterPIDKp = 0.0f;
        constexpr float kCenterPIDKi = 0.0f;
        constexpr float kCenterPIDKd = 0.0f;
        constexpr float kCenterPIDOutputMin = -255.0f;
        constexpr float kCenterPIDOutputMax = 255.0f;

        constexpr float kCenterOffsetX = 0.0f;
        constexpr float kCenterOffsetY = 10.0f;
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