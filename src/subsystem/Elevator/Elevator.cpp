/**
 * Written by Juan Pablo Gutierrez
 *
 * 25 10 2025
 */

#include "Elevator.h"

Elevator::Elevator() : elevator_state_(0), ina1(Pins::kElevatorINA1), ina2(Pins::kElevatorINA2), pwm(Pins::kElevatorPWM)
{
}

void Elevator::begin()
{
    pinMode(ina1, OUTPUT);
    pinMode(ina2, OUTPUT);
    pinMode(pwm, OUTPUT);
}

void Elevator::update()
{
    switch (elevator_state_)
    {
    case STOP:
        analogWrite(pwm, 0);
        break;
    case UP:
        moveElevator(UP);
        break;
    case DOWN:
        moveElevator(DOWN);
        break;
    }
}

void Elevator::setState(int state)
{
    elevator_state_ = static_cast<ElevatorState>(state);
}

void Elevator::moveElevator(int direction)
{
    if (direction == UP)
    {
        digitalWrite(ina1, HIGH);
        digitalWrite(ina2, LOW);
        analogWrite(pwm, 255); // Full speed up
    }
    else if (direction == DOWN)
    {
        digitalWrite(ina1, LOW);
        digitalWrite(ina2, HIGH);
        analogWrite(pwm, 255); // Full speed down
    }
}