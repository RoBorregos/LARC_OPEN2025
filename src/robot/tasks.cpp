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
TaskHandle_t elevatorTaskHandle;

// Queues for inter-task communication
QueueHandle_t stateCommandQueue;
QueueHandle_t driveCommandQueue;
QueueHandle_t elevatorCommandQueue;

void setupTasks()
{
    // Create communication structures
    stateCommandQueue = xQueueCreate(5, sizeof(StateCommand));
    driveCommandQueue = xQueueCreate(3, sizeof(DriveCommand));
    elevatorCommandQueue = xQueueCreate(3, sizeof(ElevatorCommand));
    
    // Create tasks
    xTaskCreate(driveTask, "Drive Task", 2048, NULL, 2, &driveTaskHandle);
    xTaskCreate(elevatorTask, "Elevator Task", 2048, NULL, 2, &elevatorTaskHandle);

    Serial.println("Tasks setup complete");

}

/// ----------------------------- Subsystems Tasks -----------------------------

void driveTask(void *pvParameters)
{
    DriveCommand driveCommand;
    
    while (true)
    {
        drive_.update();

        if (xQueueReceive(driveCommandQueue, &driveCommand, 0) == pdTRUE)
        {
            Serial.println("Drive command received");
            drive_.setState(driveCommand.state);
            drive_.acceptInput(driveCommand.left, driveCommand.right, driveCommand.omega);
            drive_.acceptHeadingInput(driveCommand.heading);
        }

        vTaskDelay(pdMS_TO_TICKS(SystemConstants::kUpdateInterval));
    }
}

void elevatorTask(void *pvParameters)
{
    ElevatorCommand elevatorCommand;
    
    while (true)
    {
        elevator_.update();

        if (xQueueReceive(elevatorCommandQueue, &elevatorCommand, 0) == pdTRUE)
        {
            Serial.println("Elevator command received");
            elevator_.setState(elevatorCommand.state);
        }

        vTaskDelay(pdMS_TO_TICKS(SystemConstants::kUpdateInterval));
    }
}

/// ----------------------------- Communication Functions -----------------------------

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

bool sendElevatorCommand(int state)
{
    ElevatorCommand command = {
        .state = state
    };
    
    return xQueueSend(elevatorCommandQueue, &command, 0) == pdTRUE;
}