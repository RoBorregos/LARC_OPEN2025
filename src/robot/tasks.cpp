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
TaskHandle_t globalUpdateTaskHandle;

// Queues for inter-task communication
QueueHandle_t stateCommandQueue;
QueueHandle_t driveCommandQueue;
QueueHandle_t elevatorCommandQueue;
QueueHandle_t gripperCommandQueue;
QueueHandle_t lowerSorterCommandQueue;
QueueHandle_t upperSorterCommandQueue;

void setupTasks()
{
    // Create communication structures
    stateCommandQueue = xQueueCreate(5, sizeof(StateCommand));
    driveCommandQueue = xQueueCreate(3, sizeof(DriveCommand));
    elevatorCommandQueue = xQueueCreate(3, sizeof(ElevatorCommand));
    gripperCommandQueue = xQueueCreate(3, sizeof(GripperCommand));
    lowerSorterCommandQueue = xQueueCreate(3, sizeof(LowerSorterCommand));
    upperSorterCommandQueue = xQueueCreate(3, sizeof(UpperSorterCommand));
    
    // Create tasks
    xTaskCreate(globalUpdateTask, "Global Update Task", 2048, NULL, 2, &globalUpdateTaskHandle);

    Serial.println("Tasks setup complete");
}

/// ----------------------------- Subsystems Tasks -----------------------------

void globalUpdateTask(void *pvParameters)
{
    DriveCommand driveCommand;
    ElevatorCommand elevatorCommand;
    GripperCommand gripperCommand;
    LowerSorterCommand lowerSorterCommand;
    UpperSorterCommand upperSorterCommand;

    while (true)
    {
        drive_.update();
        elevator_.update();
        gripper_.update();
        lower_sorter_.update();
        upper_sorter_.update();
        
        if (xQueueReceive(driveCommandQueue, &driveCommand, 0) == pdTRUE)
        {
            Serial.println("Drive command received");
            drive_.setState(driveCommand.state);
            drive_.acceptInput(driveCommand.left, driveCommand.right, driveCommand.omega);
            drive_.acceptHeadingInput(driveCommand.heading);
        }

        if (xQueueReceive(elevatorCommandQueue, &elevatorCommand, 0) == pdTRUE)
        {
            Serial.println("Elevator command received");
            elevator_.setState(elevatorCommand.state);
        }
        
        if (xQueueReceive(gripperCommandQueue, &gripperCommand, 0) == pdTRUE)
        {
            Serial.println("Gripper command received");
            gripper_.setState(gripperCommand.state);
        }
        
        if (xQueueReceive(lowerSorterCommandQueue, &lowerSorterCommand, 0) == pdTRUE)
        {
            Serial.println("Lower sorter command received");
            lower_sorter_.setState(lowerSorterCommand.state);
        }
        
        if (xQueueReceive(upperSorterCommandQueue, &upperSorterCommand, 0) == pdTRUE)
        {
            Serial.println("Upper sorter command received");
            upper_sorter_.setState(upperSorterCommand.state);
        }
    

        Serial.println("Global update task");
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
        .state = state};

    return xQueueSend(driveCommandQueue, &command, 0) == pdTRUE;
}

bool sendElevatorCommand(int state)
{
    ElevatorCommand command = {
        .state = state};

    return xQueueSend(elevatorCommandQueue, &command, 0) == pdTRUE;
}

bool sendGripperCommand(int state)
{
    GripperCommand command = {
        .state = state};

    return xQueueSend(gripperCommandQueue, &command, 0) == pdTRUE;
}

bool sendLowerSorterCommand(int state)
{
    LowerSorterCommand command = {
        .state = state};

    return xQueueSend(lowerSorterCommandQueue, &command, 0) == pdTRUE;
}

bool sendUpperSorterCommand(int state)
{
    UpperSorterCommand command = {
        .state = state};

    return xQueueSend(upperSorterCommandQueue, &command, 0) == pdTRUE;
}