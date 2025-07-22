/*
 * @file Elevador.cpp
 * @date 25/04/2025
 * @author Brisma Alvarez Valdez
 *
 * @brief Implementation of the Elevador class to control a stepper motor
 */
#include "Elevator.h"
#include <Arduino.h>

Elevator::Elevator()
    : motor_(Pins::kIna1, Pins::kIna2, Pins::kPwmElevator, ElevatorConstants::kInverted, Pins::kEncoderElevator, ElevatorConstants::kEncoderActiveState, 5),
      pid_controller_(ElevatorConstants::kP, ElevatorConstants::kI, ElevatorConstants::kD),
      limit_button_(Pins::kLimitElevator)
{
    pid_controller_.setOutputLimits(-255, 255);
    pid_controller_.setEnabled(true);

    limit_button_.setDebounceTime(50);
}

void Elevator::update()
{
    limit_button_.loop();

    current_position_ = motor_.getPositionMeters();

    double output = pid_controller_.update(current_position_, target_position_);
    motor_.move(output);

    if (getLimitState())
    {
        resetPosition(ElevatorConstants::kIdleLevel);
    }
}

void Elevator::setState(int state)
{
    int target_pos = 0;
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
    return limit_button_.isPressed();
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
