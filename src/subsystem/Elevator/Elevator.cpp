/*
 * @file Elevador.cpp
 * @date 25/04/2025
 * @author Brisma Alvarez Valdez
 *
 * @brief Implementation of the Elevador class to control a stepper motor
 */
#include "Elevator.hpp"
#include <Arduino.h>

Elevator::Elevator() : current_position_(ElevatorConstants::kIdleLevel) {
}

void Elevator::update() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        if (command.toInt() != 0 || command == "0") {
            current_position_ = command.toInt();
        }
    }
}

bool Elevator::getLimitState() {
    return digitalRead(limitPin) == LOW;
}

void Elevator::resetPosition(double position) {
    Serial.print("SET_POSITION:");
    Serial.println(position);
}

void Elevator::setTargetPosition(int position) {
    Serial.print("SET_TARGET:");
    Serial.println(position);
}

int Elevator::getCurrentPosition() {
    Serial.println("GET_POS");
    return current_position_;
}

void Elevator::setState(int state) {
    int target_pos = 0;
    switch (state) {
        case 0: // HOME
            target_pos = ElevatorConstants::kIdleLevel;
            break;
        case 1: // LOWER
            target_pos = ElevatorConstants::kLowerLevel;
            break;
        case 2: // MID
            target_pos = ElevatorConstants::kMidLevel;
            break;
        case 3: // UPPER
            target_pos = ElevatorConstants::kUpperLevel;
            break;
    }
    setTargetPosition(target_pos);
}
