/*
 * @file Gripper.cpp
 * @date 12/03/2025
 * @author Brisma Alvarez Valdez & Juan Pablo Gutierrez
 *
 * @brief Implementation of the Gripper class to control a servo and a motor.
 */

#include "UpperSorter.hpp"

UpperSorter::UpperSorter() {
    servo.attach(Pins::kUpperSorterServoPin);
    delay(100);
}

void UpperSorter::update() {
    switch (upper_sorter_state_) {
        case UpperSorterState::LEFT:
            move(UpperSorterConstants::kLeftAngle);
            break;
        case UpperSorterState::CENTER:
            move(UpperSorterConstants::kCenterAngle);
            break;
        case UpperSorterState::RIGHT:
            move(UpperSorterConstants::kRightAngle);
            break;
    }
}

void UpperSorter::setState(int state) {
    upper_sorter_state_ = static_cast<UpperSorterState>(state);
}

void UpperSorter::move(int angle) { 
    servo.write(angle);
}