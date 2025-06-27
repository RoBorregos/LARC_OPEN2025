/*
 * @file Gripper.cpp
 * @date 12/03/2025
 * @author Brisma Alvarez Valdez & Juan Pablo Gutierrez
 *
 * @brief Implementation of the Gripper class to control a servo and a motor.
 */

#include "Gripper.hpp"

Gripper::Gripper() {
    servo.attach(Pins::kGripperServoPin);
    delay(100);
}

void Gripper::update() {
    switch (gripper_state_) {
        case GripperState::CLOSED:
            move(GripperConstants::kClosedAngle);
            break;
        case GripperState::OPEN:
            move(GripperConstants::kOpenAngle);
            break;
    }
}

void Gripper::setState(int state) {
    gripper_state_ = static_cast<GripperState>(state);
}

void Gripper::move(int angle) { 
    servo.write(angle);
}