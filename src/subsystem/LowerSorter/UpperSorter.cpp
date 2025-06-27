/*
 * @file Gripper.cpp
 * @date 12/03/2025
 * @author Brisma Alvarez Valdez & Juan Pablo Gutierrez
 *
 * @brief Implementation of the Gripper class to control a servo and a motor.
 */

#include "LowerSorter.hpp"

LowerSorter::LowerSorter() {
    servo.attach(Pins::kLowerSorterServoPin);
    delay(100);
}

void LowerSorter::update() {
    switch (lower_sorter_state_) {
        case LowerSorterState::LEFT:
            move(LowerSorterConstants::kLeftAngle);
            break;
        case LowerSorterState::CENTER:
            move(LowerSorterConstants::kCenterAngle);
            break;
        case LowerSorterState::RIGHT:
            move(LowerSorterConstants::kRightAngle);
            break;
    }
}

void LowerSorter::setState(int state) {
    lower_sorter_state_ = static_cast<LowerSorterState>(state);
}

void LowerSorter::move(int angle) { 
    servo.write(angle);
}