/*
 * @file Sorter.h
 * @date 05/05/2025
 * @author  Juan Pablo Gutierrez
 *
 * @brief Head file of the Sorter class to control a servo and a motor.
 */

#pragma once

#include <Arduino.h>
#include <PWMServo.h>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"

using namespace Constants;

class LowerSorter : public System {
    public:
        LowerSorter();
        void update() override;
        void setState(int state) override;
    private:
        PWMServo servo;
        void move(int angle);
        enum class LowerSorterState {
            LEFT = 0,
            CENTER = 1,
            RIGHT = 2,
        };

        LowerSorterState lower_sorter_state_ = LowerSorterState::CENTER;
};