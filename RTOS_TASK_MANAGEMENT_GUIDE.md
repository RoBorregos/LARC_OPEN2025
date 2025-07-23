# RTOS Task Management and Communication Guide

## Overview

This guide explains the recommended architecture for managing RTOS tasks and communication between them, specifically designed for a state machine-based robot control system.

## Key Principles

### 1. Task Creation Strategy

**❌ DON'T: Dynamic Task Creation**
```cpp
// BAD: Creating tasks during runtime
void someTask(void *pvParameters) {
    if (condition) {
        xTaskCreate(anotherTask, "Another", 1024, NULL, 1, NULL);
    }
}
```

**✅ DO: Static Task Creation**
```cpp
// GOOD: All tasks created at initialization
void setupTasks() {
    xTaskCreate(stateManagerTask, "State Manager", 4096, NULL, 3, &stateManagerTaskHandle);
    xTaskCreate(globalUpdateTask, "Global Update", 2048, NULL, 2, &globalUpdateTaskHandle);
    xTaskCreate(communicationTask, "Communication", 2048, NULL, 1, &communicationTaskHandle);
    xTaskCreate(visionTask, "Vision", 3072, NULL, 2, &visionTaskHandle);
}
```

### 2. Task Priority Hierarchy

```
Priority 3: State Manager Task (Highest)
- Manages state transitions
- Coordinates all subsystems
- Handles critical decisions

Priority 2: Control Tasks (Medium-High)
- Global Update Task (subsystem updates)
- Vision Task (sensor processing)

Priority 1: Communication Tasks (Medium)
- External communication
- Data logging
- Non-critical operations
```

## Communication Patterns

### 1. Queue-Based Communication

**State Commands**
```cpp
struct StateCommand {
    RobotState targetState;
    uint32_t timestamp;
    bool isPriority;
};

// Send state command
bool sendStateCommand(RobotState state, bool priority = false) {
    StateCommand command = {
        .targetState = state,
        .timestamp = millis(),
        .isPriority = priority
    };
    return xQueueSend(stateCommandQueue, &command, 
                     priority ? pdMS_TO_TICKS(100) : 0) == pdTRUE;
}
```

**Subsystem Status**
```cpp
struct SubsystemStatus {
    bool driveReady;
    bool elevatorReady;
    bool gripperReady;
    bool visionReady;
    uint32_t timestamp;
};
```

### 2. Mutex Protection for Shared Resources

```cpp
// Protect shared resources
if (xSemaphoreTake(driveMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
    drive_.update();
    xSemaphoreGive(driveMutex);
}
```

### 3. Event-Driven Communication

```cpp
// Vision task detects obstacle
void visionTask(void *pvParameters) {
    while (true) {
        if (detectObstacle()) {
            sendStateCommand(RobotState::AVOID_POOL, true);
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
```

## State Machine Architecture

### 1. Centralized State Management

```cpp
void stateManagerTask(void *pvParameters) {
    RobotState currentState = RobotState::INIT;
    
    while (true) {
        // Check for state change commands
        StateCommand command;
        if (xQueueReceive(stateCommandQueue, &command, pdMS_TO_TICKS(10)) == pdTRUE) {
            if (isValidStateTransition(currentState, command.targetState)) {
                currentState = command.targetState;
                notifySubsystemsOfStateChange(currentState);
            }
        }
        
        // Execute current state logic
        executeStateLogic(currentState);
        
        vTaskDelay(pdMS_TO_TICKS(SystemConstants::kUpdateInterval));
    }
}
```

### 2. State Transition Validation

```cpp
bool isValidStateTransition(RobotState from, RobotState to) {
    // Define valid state transitions
    switch (from) {
        case RobotState::INIT:
            return to == RobotState::EXIT_START;
        case RobotState::EXIT_START:
            return to == RobotState::GO_TREES || to == RobotState::IDLE;
        case RobotState::GO_TREES:
            return to == RobotState::SEARCH_TREES || to == RobotState::AVOID_POOL;
        // ... define other transitions
        default:
            return false;
    }
}
```

## Task Responsibilities

### State Manager Task
- **Priority**: 3 (Highest)
- **Stack Size**: 4096 bytes
- **Responsibilities**:
  - Manage state transitions
  - Validate state changes
  - Coordinate subsystem activities
  - Handle emergency states

### Global Update Task
- **Priority**: 2
- **Stack Size**: 2048 bytes
- **Responsibilities**:
  - Update all subsystems
  - Monitor subsystem health
  - Send status updates
  - Handle low-level control

### Communication Task
- **Priority**: 1
- **Stack Size**: 2048 bytes
- **Responsibilities**:
  - Handle external communication
  - Process vision data
  - Log system events
  - Manage remote commands

### Vision Task
- **Priority**: 2
- **Stack Size**: 3072 bytes
- **Responsibilities**:
  - Process camera data
  - Detect obstacles
  - Identify targets
  - Send vision commands

## Best Practices

### 1. Task Design
- Keep tasks focused on single responsibility
- Use appropriate stack sizes
- Set reasonable priorities
- Avoid blocking operations

### 2. Communication
- Use queues for asynchronous communication
- Use mutexes for shared resources
- Avoid global variables
- Implement timeout mechanisms

### 3. Error Handling
- Always check return values
- Implement timeout handling
- Use watchdog timers
- Log error conditions

### 4. Performance
- Minimize task switching overhead
- Use appropriate delay times
- Monitor stack usage
- Profile task execution times

## Example Implementation

```cpp
// Task creation with proper error handling
void setupTasks() {
    BaseType_t result;
    
    // Create communication structures
    stateCommandQueue = xQueueCreate(5, sizeof(StateCommand));
    if (stateCommandQueue == NULL) {
        Serial.println("Failed to create state command queue");
        return;
    }
    
    // Create tasks
    result = xTaskCreate(stateManagerTask, "State Manager", 4096, NULL, 3, &stateManagerTaskHandle);
    if (result != pdPASS) {
        Serial.println("Failed to create state manager task");
        return;
    }
    
    // ... create other tasks
}
```

## Debugging Tips

1. **Monitor Task Status**
```cpp
void printTaskStatus() {
    Serial.printf("State Manager: %d\n", eTaskGetState(stateManagerTaskHandle));
    Serial.printf("Global Update: %d\n", eTaskGetState(globalUpdateTaskHandle));
}
```

2. **Check Queue Status**
```cpp
void printQueueStatus() {
    Serial.printf("State Commands: %d/%d\n", 
                 uxQueueMessagesWaiting(stateCommandQueue),
                 uxQueueSpacesAvailable(stateCommandQueue));
}
```

3. **Monitor Stack Usage**
```cpp
void printStackUsage() {
    Serial.printf("State Manager Stack: %d\n", 
                 uxTaskGetStackHighWaterMark(stateManagerTaskHandle));
}
```

This architecture provides a robust, predictable, and maintainable foundation for your robot's state machine system. 