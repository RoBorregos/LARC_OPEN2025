/**
 * Written by Juan Pablo Gutierrez
 *
 * 25 10 2024
 */

#pragma once
#include "../systems/system.hpp"
#include "constants/pins.h"

class Elevator : public System
{
public:
    Elevator();
    void begin();
    void update();
    void setState(int state);

private:
    int elevator_state_;

    int ina1;
    int ina2;
    int pwm;

    enum ElevatorState
    {
        STOP = 0,
        UP = 1,
        DOWN = 2,
    };

    void moveElevator(int direction);
};