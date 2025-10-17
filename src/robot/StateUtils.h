/**
 * @file StateUtils.h
 * @date 30/09/2025
 * @author Juan Pablo Gutiérrez
 *
 * @brief Utility functions for the state machine.
 */

#pragma once

#include "../subsystem/DistanceSensors/DistanceSensor.hpp"
#include "constants/constants.h"
#include "../../lib/controllers/PIDController.hpp"
#include "robot/robot_instances.h"
#include "../subsystem/LineSensor/LineSensor.hpp"

using namespace Constants;

// Declare external PID controllers (defined in StateUtils.cpp)
extern PIDController leftDistancePID;
extern PIDController rightDistancePID;
extern PIDController followLinePID;
extern PIDController lateralPID;

// Declare constants
extern const float LATERAL_SPEED;

// Function declarations
void maintainDistance(float distance, float lateralSpeed);
void followLine(float lateralSpeed);
void evadeLine(float lateralSpeed);
