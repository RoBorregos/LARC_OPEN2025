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

// Forward declarations for state task functions
void exitInitTask();
void goTreesTask();
void pickLowLevelTask();
void pickMiddleLevelTask();
void goToStorageTask();
void InitTask();

// Task handles
TaskHandle_t stateManageTaskHandle;
TaskHandle_t driveTaskHandle;
TaskHandle_t elevatorTaskHandle;
TaskHandle_t gripperTaskHandle;
TaskHandle_t lowerSorterTaskHandle;
TaskHandle_t upperSorterTaskHandle;

// Queues for inter-task communication
QueueHandle_t stateCommandQueue;
QueueHandle_t driveCommandQueue;
QueueHandle_t elevatorCommandQueue;
QueueHandle_t gripperCommandQueue;
QueueHandle_t lowerSorterCommandQueue;
QueueHandle_t upperSorterCommandQueue;

// Global variables
int beansCounter = 0;

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
    xTaskCreate(stateManageTask, "State Manager Task", 2048, NULL, 3, &stateManageTaskHandle);
    xTaskCreate(driveTask, "Drive Task", 2048, NULL, 2, &driveTaskHandle);
    xTaskCreate(elevatorTask, "Elevator Task", 2048, NULL, 2, &elevatorTaskHandle);
    xTaskCreate(gripperTask, "Gripper Task", 2048, NULL, 2, &gripperTaskHandle);
    xTaskCreate(lowerSorterTask, "Lower Sorter Task", 2048, NULL, 2, &lowerSorterTaskHandle);
    xTaskCreate(upperSorterTask, "Upper Sorter Task", 2048, NULL, 2, &upperSorterTaskHandle);

    Serial.println("Tasks setup complete");
}

/// ----------------------------- Subsystems Tasks -----------------------------

void stateManageTask(void *pvParameters)
{

    RobotState currentState = RobotState::IDLE;

    while (true)
    {
        StateCommand stateCommand;

        if (xQueueReceive(stateCommandQueue, &stateCommand, pdMS_TO_TICKS(10)) == pdTRUE)
        {
            if (isValidStateTransition(currentState, stateCommand.targetState))
            {
                currentState = stateCommand.targetState;
            }
        }

        // Perform action based on current state
        
        switch (currentState)
        {
        case RobotState::IDLE:
            break;
        case RobotState::INIT:
            Serial.println("Robot is initializing...");
            InitTask();
            break;
        case RobotState::EXIT_START:
            exitInitTask();
            break;
        case RobotState::GO_TREES:
            goTreesTask();
            break;
        case RobotState::PICK_LOW_LEVEL:
            pickLowLevelTask();
            break;
        case RobotState::PICK_MID_LEVEL:
            break;
        case RobotState::GO_STORAGES:
            goToStorageTask();
            break; 

        }

        Serial.println("Global update task");
        vTaskDelay(pdMS_TO_TICKS(SystemConstants::kUpdateInterval));
    }
}

void driveTask(void *pvParameters)
{
    DriveCommand driveCommand;
    while (true)
    {
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
        if (xQueueReceive(elevatorCommandQueue, &elevatorCommand, 0) == pdTRUE)
        {
            Serial.println("Elevator command received");
            elevator_.setState(elevatorCommand.state);
        }
        vTaskDelay(pdMS_TO_TICKS(SystemConstants::kUpdateInterval));
    }
}

void gripperTask(void *pvParameters)
{
    GripperCommand gripperCommand;
    while (true)
    {
        if (xQueueReceive(gripperCommandQueue, &gripperCommand, 0) == pdTRUE)
        {
            Serial.println("Gripper command received");
            gripper_.setState(gripperCommand.state);
        }
        vTaskDelay(pdMS_TO_TICKS(SystemConstants::kUpdateInterval));
    }
}

void lowerSorterTask(void *pvParameters)
{
    LowerSorterCommand lowerSorterCommand;
    while (true)
    {
        if (xQueueReceive(lowerSorterCommandQueue, &lowerSorterCommand, 0) == pdTRUE)
        {
            Serial.println("Lower sorter command received");
            lower_sorter_.setState(lowerSorterCommand.state);
        }
        vTaskDelay(pdMS_TO_TICKS(SystemConstants::kUpdateInterval));
    }
}

void upperSorterTask(void *pvParameters)
{
    UpperSorterCommand upperSorterCommand;
    while (true)
    {
        if (xQueueReceive(upperSorterCommandQueue, &upperSorterCommand, 0) == pdTRUE)
        {
            Serial.println("Upper sorter command received");
            upper_sorter_.setState(upperSorterCommand.state);
        }
        vTaskDelay(pdMS_TO_TICKS(SystemConstants::kUpdateInterval));
    }
}

/// ----------------------------- State Tasks -----------------------------

void InitTask()
{
    // Initialize subsystems
    sendDriveCommand(0, 0, 0, Rotation2D(0), 0);
    sendElevatorCommand(2);
    sendGripperCommand(1);

    // Set initial state
    sendStateCommand(RobotState::EXIT_START);
    Serial.println("Initialization complete, moving to EXIT_START state");
}

void exitInitTask()
{
    if(com_.getCommand() == "START"){
        sendDriveCommand(0, 200, 0, Rotation2D(0), 0);

        if (distance_sensor_.getLeftDistance() < 15 && distance_sensor_.getRightDistance() < 15)
        {
            sendDriveCommand(0, 0, 0, Rotation2D(0), 0);
            sendStateCommand(RobotState::PICK_LOW_LEVEL);
            Serial.println("Exiting init task, found a tree");
        }
        else
        {
            Serial.println("Waiting for found a tree");
        }
    }else{
        Serial.println("No command received, staying in EXIT_START state");
        sendStateCommand(RobotState::EXIT_START);
    }
}

void goTreesTask(){
    sendDriveCommand(0, 200, 0, Rotation2D(0), 0);
    if(com_.getCommand() == "Tree"){

    }
}

void pickLowLevelTask()
{
    sendElevatorCommand(2);
    sendGripperCommand(1);

   if(beansCounter <= 5){
        if (distance_sensor_.getGripperDistance() < 10)
        {
            sendGripperCommand(0);
            vTaskDelay(pdMS_TO_TICKS(2000)); // Wait for the gripper to close
            Serial.println("PICK LOOW LEVEL TASK: Found a bean");
            sendElevatorCommand(0);
            sendGripperCommand(1);
            sendLowerSorterCommand(0);
            vTaskDelay(pdMS_TO_TICKS(2000)); // Wait for the gripper to close
            sendLowerSorterCommand(1);
            sendElevatorCommand(2);
            beansCounter++;
        }
        else
        {
            drive_.acceptInput(50, 50, 0);
            Serial.println("Waiting for finding a bean");
        }
    }
    else{
        sendStateCommand(RobotState::PICK_MID_LEVEL);
        Serial.println("Exiting pick low level task, moving to picking mid level state");
    }
}

void pickMiddleLevelTask(){
    sendElevatorCommand(3);
    sendGripperCommand(1);

    if(beansCounter <= 3){
        if (distance_sensor_.getGripperDistance() < 10)
        {
            sendGripperCommand(0);
            vTaskDelay(pdMS_TO_TICKS(2000)); // Wait for the gripper to close
            Serial.println("PICK MID LEVEL TASK: Found a bean");
            sendElevatorCommand(0);
            sendGripperCommand(1);
            sendLowerSorterCommand(0);
            vTaskDelay(pdMS_TO_TICKS(2000)); // Wait for the gripper to close
            sendLowerSorterCommand(1);
            sendElevatorCommand(1);
            beansCounter++;
        }
        else
        {
            drive_.acceptInput(50, 50, 0);
            Serial.println("Waiting for finding a bean");
        }
    }
    else{
        sendStateCommand(RobotState::GO_STORAGES);
        Serial.println("Exiting pick mid level task, moving to Go Storages state");
    }
}

void goToStorageTask()
{
    // This task would contain logic to navigate to the storage area
    // For now, we will just simulate it with a delay
    Serial.println("Going to storage...");
    sendDriveCommand(0,-200,0,Rotation2D(0),0);
    vTaskDelay(pdMS_TO_TICKS(3000));
    sendDriveCommand(0,0,0,Rotation2D(0),0);
    vTaskDelay(pdMS_TO_TICKS(5000));

    Serial.println("Reached storage, dropping beans...");
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

bool sendStateCommand(RobotState state)
{
    StateCommand command = {.targetState = state};
    return xQueueSend(stateCommandQueue, &command, 0) == pdTRUE;
}

/// ----------------------------- State Helper Functions -----------------------------

bool isValidStateTransition(RobotState currentState, RobotState targetState)
{
    switch (currentState)
    {
    default:
        return false;
    }
}