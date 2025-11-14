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
extern PIDController followLinePID;
extern PIDController lateralPID;

// Declare constants
extern const float LATERAL_SPEED;

// Function declarations
void maintainDistance(float distance, float lateralSpeed);
void followLineHybrid(float lateralSpeed, float dt);
void followLineImpulse(float lateralSpeed);


// Global or static variable to remember the last detected sensor
enum LastSensor { NONE, LEFT, CENTER, RIGHT };
// Declaration only: single definition lives in StateUtils.cpp
extern LastSensor last_detected;

// Function prototype (fixed parameter name)
void followLine(float lateralSpeed);
void followLineCombined(float lateralSpeed, float dt);