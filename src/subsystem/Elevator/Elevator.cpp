/*
 * @file Elevador.cpp
 * @date 25/04/2025
 * @author Brisma Alvarez Valdez
 *
 * @brief Implementation of the Elevador class to control a stepper motor
 */
#include "Elevator.hpp"
#include <Arduino.h>

Elevator::Elevator()
{
}

void Elevator::begin()
{
}

void Elevator::update()
{

    if (getLimitState())
    {
        resetPosition(ElevatorConstants::kIdleLevel);
    }
}

void Elevator::setState(int state)
{
    switch (state)
    {
    case 0: // HOME
        target_position_ = ElevatorConstants::kIdleLevel;
        break;
    case 1: // LOWER
        target_position_ = ElevatorConstants::kLowerLevel;
        break;
    case 2: // MID
        target_position_ = ElevatorConstants::kMidLevel;
        break;
    case 3: // UPPER
        target_position_ = ElevatorConstants::kUpperLevel;
        break;
    }
}

bool Elevator::getLimitState()
{
}

void Elevator::resetPosition(double position)
{
    Serial.println(position);
    current_position_ = position;
}

int Elevator::getCurrentPosition()
{
    return current_position_;
}