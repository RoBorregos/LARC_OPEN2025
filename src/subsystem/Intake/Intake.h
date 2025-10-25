#ifndef INTAKE_H
#define INTAKE_H

#include <Arduino.h>
#include <Servo.h>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"

using namespace Constants;

class Intake : public System
{
private:
    Servo UpperIntakeServo;
    Servo LowerIntakeServo;
    Servo IntakeRampEnableServo;

    void setIntakeServoPosition(Servo &servo, int position);
public:
    Intake();

    void begin() override;
    void update() override;
    void setState(int state) override;
    
    enum class IntakeState
    {
        ALL_SERVOS_STORED = 0,
        ALL_SERVOS_POSITIONED = 1,
        UPPER_SERVO_ONLY_STORED = 2,
        LOWER_SERVO_ONLY_STORED = 3,
        UPPER_SERVO_ONLY_POSITIONED = 4,
        LOWER_SERVO_ONLY_POSITIONED = 5,
    };

    IntakeState intake_state_ = IntakeState::ALL_SERVOS_STORED;
};

#endif // INTAKE_H