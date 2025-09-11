#pragma once

#include <Arduino.h>
#include <PWMServo.h>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"

using namespace Constants;

class Intake : public System
{
public:
    Intake();
    void update() override;
    void setState(int state) override;

private:
    PWMServo upper;
    PWMServo middle;
    PWMServo lower;
    void move(int angle);
    enum class IntakeState
    {
        COLLECT = 0,
        IDLE = 1,
    };

    IntakeState intake_state = IntakeState::IDLE;
};