#include "Elevator.hpp"

//* Constructor
Elevator::Elevator(int stepPin, int dirPin)
    : step_pin_(stepPin), dir_pin_(dirPin) {}

//* Setup method to initialize the motors
bool Elevator::setup() {
    elevator_state_ = ElevatorState::ON;
    Serial.println("Elevator initialized.");
    return true; // Return true when setup is successful
}

//* Method to move the elevator up or down depending on the goal step count
void Elevator::moveToStepCount(int goalSteps) {
    if (current_step_count_ == goalSteps)
        return;
    
    // Calculate distance to travel
    int steps = abs(goalSteps - current_step_count_);

    // Determine direction
    if (goalSteps > current_step_count_) {
        digitalWrite(dir_pin_, HIGH);  // Up
    } else {
        digitalWrite(dir_pin_, LOW);   // Down
    }

    // Perform step movements
    for (int i = 0; i < steps; i++) {
        digitalWrite(step_pin_, HIGH);
        delayMicroseconds(1000);

        digitalWrite(step_pin_, LOW);
        delayMicroseconds(1000);
    }

    // Update current position
    current_step_count_ = goalSteps;
}

//* Method to update elevator height if needed
void Elevator::update() {
    switch (elevator_state_) {
        case ElevatorState::OFF:
            moveToStepCount(0);
            break;
        case ElevatorState::ON:
            moveToStepCount(kONStepCount);
            break;
    }
}

void Elevator::setState(int state) {
    elevator_state_ = static_cast<ElevatorState>(state);
}