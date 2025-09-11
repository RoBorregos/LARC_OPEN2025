#pragma once

#include <Arduino.h>
#include "constants/constants.h"
#include "../systems/system.hpp"
#include "constants/pins.h"

using namespace Constants;

class Elevator : public System
{

public:
    Elevator();
    void begin() override;

    void update() override;
    void setState(int state) override;
    int getCurrentPosition();

private:
    void Move(int steps);
    void resetPosition(double position);

    enum class ElevatorState
    {
        HOME = 0,
        LOWER = 2,
        MID = 3,
        UPPER = 4,
    };

    ElevatorState elevator_state_ = ElevatorState::HOME;

    int current_position_ = 0;
    int target_position_ = 0;
};