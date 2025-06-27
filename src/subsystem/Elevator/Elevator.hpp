/*
 * @file Elevador.h
 * @date 25/04/2025
 * @author Brisma Alvarez Valdez
 *
 * @brief Head file of the Elevador class to control a stepper motor.
 */

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

    void update() override;           
    void setState(int state) override;
    void setTargetPosition(int position);
    int getCurrentPosition();

private:
    float actual_position_cm;
    float target_position_cm;

    void Move(int steps);
    void resetPosition(double position);
    bool getLimitState();

    // States
    enum class ElevatorState
    {
        HOME = 0,
        LOWER = 2,
        MID = 3,
        UPPER = 4,
    };

    ElevatorState elevator_state_ = ElevatorState::HOME;

    int current_position_ = 0;
    int limitPin = -1;
};
