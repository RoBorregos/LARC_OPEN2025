#pragma once

#include "PID_v1.h"
#include "robot_instances.h"
#include "../lib/math/pose2d.hpp"
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include "../include/constants/constants.h"
#include "../include/RobotState.h"

using namespace Constants;

// Task handles
extern TaskHandle_t stateManagerTaskHandle;
extern TaskHandle_t driveTaskHandle;

// Queues for inter-task communication
extern QueueHandle_t stateCommandQueue;
extern QueueHandle_t driveCommandQueue;

// Task function declarations
void setupTasks();
void driveTask(void *pvParameters);

// Communication helper functions
bool sendDriveCommand(float left, float right, float omega, Rotation2D heading, int state);
// Communication structures

struct StateCommand
{
    RobotState targetState;
    uint32_t timestamp;
    bool isPriority;
};

struct DriveCommand
{
    float left;
    float right;
    float omega;
    Rotation2D heading;
    int state;
};