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
    Servo UpperIntakeServo;
    Servo LowerIntakeServo;
    Servo IntakeRampEnableServo;

    // interval (ms) between calls to com_.getMatrix()
    static constexpr unsigned long recive_values_interval = 800UL;
    // store last received timestamp using unsigned long to match millis()
    unsigned long last_value_received = 0;

    void setIntakeServoPosition(Servo &servo, int position);
    Communication com_;
public:

    std::vector<int> values = {0, 0};
    Intake();

    void begin() override;
    void update() override;
    void setState(int state) override;
    void vision();
    
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