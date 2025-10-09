#include "Sorter.hpp"

Elevator::Elevator()
    : motor_pin_(Pins::kElevatorStepPin) {}

void Elevator::begin() {
    pinMode(motor_pin_, OUTPUT);
    deactivateMotor();
}

void Elevator::activateMotor() {
    digitalWrite(motor_pin_, HIGH);
    elevator_state_ = ElevatorState::ACTIVE;
}

void Elevator::deactivateMotor() {
    digitalWrite(motor_pin_, LOW);
    elevator_state_ = ElevatorState::INACTIVE;
}

bool Elevator::isActive() const {
    return elevator_state_ == ElevatorState::ACTIVE;
}

void Elevator::update() {
}

void Elevator::setState(int state) {
    if (state == 1) {
        activateMotor();
    } else {
        deactivateMotor();
    }
}