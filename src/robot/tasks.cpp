/**
 * @file tasks.cpp
 * @brief File for the tasks of the robot
 * @author Juan Pablo Gutiérrz
 * @date 2025-07-16
 */
#include "tasks.h"
#include "robot_instances.h"
#include "../lib/math/rotation2d.hpp"
#include "../include/RobotState.h"

// Task handles
TaskHandle_t stateManagerTaskHandle;
TaskHandle_t driveTaskHandle;

// Queues for inter-task communication
QueueHandle_t stateCommandQueue;
QueueHandle_t driveCommandQueue;

void setupTasks()
{
    // Create communication structures
    stateCommandQueue = xQueueCreate(5, sizeof(StateCommand));
    driveCommandQueue = xQueueCreate(3, sizeof(DriveCommand));
    
    // Create tasks
    xTaskCreate(driveTask, "Drive Task", 2048, NULL, 2, &driveTaskHandle);
}

void driveTask(void *pvParameters)
{
    DriveCommand driveCommand;
    
    while (true)
    {
        drive_.update();

        if (xQueueReceive(driveCommandQueue, &driveCommand, 0) == pdTRUE)
        {
            drive_.setState(driveCommand.state);
            drive_.acceptInput(driveCommand.left, driveCommand.right, driveCommand.omega);
            drive_.acceptHeadingInput(driveCommand.heading);
        }

        vTaskDelay(pdMS_TO_TICKS(SystemConstants::kUpdateInterval));
    }
}


bool sendDriveCommand(float left, float right, float omega, Rotation2D heading, int state)
{
    DriveCommand command = {
        .left = left,
        .right = right,
        .omega = omega,
        .heading = heading,
        .state = state
    };
    
    return xQueueSend(driveCommandQueue, &command, 0) == pdTRUE;
}
