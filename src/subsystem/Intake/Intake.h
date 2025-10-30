#ifndef INTAKE_H
#define INTAKE_H

#include <Arduino.h>
#include <Servo.h>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"
#include "subsystem/Communication/Communication.hpp"

using namespace Constants;

class Intake : public System
{
private:
    Servo servo;

    // interval (ms) between calls to com_.getMatrix()
    static constexpr unsigned long recive_values_interval = 800UL;
    // store last received timestamp using unsigned long to match millis()
    unsigned long last_value_received = 0;

    void setIntakeServoPosition(int position);
    Communication com_;

public:
    Intake(int pin_, int grabPosition_, int storedPosition_);

    int pin;
    int grabPosition = 0;
    int storedPosition = 0;

    void begin() override;
    void update() override;
    void setState(int state) override;

    enum class IntakeState
    {
        STORED = 0,
        POSITIONED = 1,
    };

    IntakeState intake_state_ = IntakeState::STORED;
};

#endif // INTAKE_H