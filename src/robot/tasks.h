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
extern TaskHandle_t stateManageTaskHandle;
extern TaskHandle_t driveTaskHandle;
extern TaskHandle_t elevatorTaskHandle;
extern TaskHandle_t gripperTaskHandle;
extern TaskHandle_t lowerSorterTaskHandle;
extern TaskHandle_t upperSorterTaskHandle;

// Queues for inter-task communication
extern QueueHandle_t stateCommandQueue;
extern QueueHandle_t driveCommandQueue;
extern QueueHandle_t elevatorCommandQueue;
extern QueueHandle_t gripperCommandQueue;
extern QueueHandle_t lowerSorterCommandQueue;
extern QueueHandle_t upperSorterCommandQueue;

// Task function declarations
void setupTasks();
void stateManageTask(void *pvParameters);
void driveTask(void *pvParameters);
void elevatorTask(void *pvParameters);
void gripperTask(void *pvParameters);
void lowerSorterTask(void *pvParameters);
void upperSorterTask(void *pvParameters);

// Communication helper functions
bool sendDriveCommand(float left, float right, float omega, Rotation2D heading, int state);
bool sendElevatorCommand(int state);
bool sendGripperCommand(int state);
bool sendLowerSorterCommand(int state);
bool sendUpperSorterCommand(int state);
bool sendStateCommand(RobotState state);

// State transition helper functions
bool isValidStateTransition(RobotState currentState, RobotState targetState);

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

struct ElevatorCommand
{
    int state;
};

struct GripperCommand
{
    int state;
};

struct LowerSorterCommand
{
    int state;
};

struct UpperSorterCommand
{
    int state;
};

/// ----------------------------- Global Variables -----------------------------

int beansCounter;
