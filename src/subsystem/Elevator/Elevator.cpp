#include "Elevator.hpp"

//* Constructor
Elevator::Elevator(int stepPin, int dirPin)
    : left_step_pin_(stepPin), left_dir_pin_(dirPin),
      right_step_pin_(stepPin), right_dir_pin_(dirPin) {}

//* Setup method to initialize the motors
bool Elevator::setup() {
    //moveDown(kUpperLevel); // Ensure that the elevator is at the bottom at the start of the game
    current_height_ = ElevatorConstants::kUpperLevel; //para pruebas del sábado
    Serial.println("Elevator initialized.");
    return true; // Return true when setup is successful
}

void Elevator::update() {
    switch (elevator_state_) {
        case ElevatorState::HOME:
            moveToHeight(ElevatorConstants::kUpperLevel);
            break;
        case ElevatorState::LOW_POSITION:
            moveToHeight(ElevatorConstants::kLowerLevel);
            break;
        case ElevatorState::MID_POSITION:
            moveToHeight(ElevatorConstants::kMidLevel);
            break;
        case ElevatorState::HIGH_POSITION:
            moveToHeight(ElevatorConstants::kUpperLevel);
            break;
    }
}

void Elevator::setState(int state) {
    elevator_state_ = static_cast<ElevatorState>(state);
}

//* Method to move the elevator up by a specified distance in cm
void Elevator::moveUp(float distance) {
    int steps = translateCmToSteps(distance); // Convert distance to steps

    // Serial.print("Elevator moving up: ");
    // Serial.print(distance);
    // Serial.println(" cm");

    digitalWrite(left_dir_pin_, HIGH);
    digitalWrite(right_dir_pin_, LOW);
    
    for (int i = 0; i < steps; i++) {
        digitalWrite(left_step_pin_, HIGH);
        digitalWrite(right_step_pin_, HIGH);
        delayMicroseconds(1000);

        digitalWrite(left_step_pin_, LOW);
        digitalWrite(right_step_pin_, LOW);
        delayMicroseconds(1000);
    }
}

//* Method to move the elevator down by a specified distance in cm
void Elevator::moveDown(float distance) {
    int steps = translateCmToSteps(distance); // Convert distance to steps

    // Serial.print("Elevator moving down: ");
    // Serial.print(distance);
    // Serial.println(" cm");

    digitalWrite(left_dir_pin_, LOW);
    digitalWrite(right_dir_pin_, HIGH);
    
    for (int i = 0; i < steps; i++) {
        digitalWrite(left_step_pin_, HIGH);
        digitalWrite(right_step_pin_, HIGH);
        delayMicroseconds(1000);
        digitalWrite(left_step_pin_, LOW);
        digitalWrite(right_step_pin_, LOW);
        delayMicroseconds(1000);
    }
}

//* Method to move to a specific height in cm
void Elevator::moveToHeight(float height) {
    if (height > current_height_) {
        moveUp(height - current_height_);
    } else if (height < current_height_) {
        moveDown(current_height_ - height);
    }
    current_height_ = height; // Update the current height
}

//* Stop the motors
void Elevator::stop() {
    // Serial.println("Elevator stops");

    digitalWrite(left_step_pin_, LOW);
    digitalWrite(right_step_pin_, LOW);
}

/* Private methods */

//* Method to convert a distance in centimeters to the corresponding number of steps. Returns number of steps required to move the given distance
int Elevator::translateCmToSteps(float distance) {
    return (int)((distance / (2 * M_PI * ElevatorConstants::kStepperRadius)) * ElevatorConstants::kStepsPerRevolution);
}