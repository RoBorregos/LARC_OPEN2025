#ifndef INTAKE_H
#define INTAKE_H

#include <Arduino.h>
#include <Servo.h>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"

using namespace Constants;

class Intake : public System {
private:
    
    static constexpr int kGrabBallServoPosition = 0;
    static constexpr int kAvoidBallServoPosition = 180;
    static constexpr int kIntakeRampPositioned = 90;
    static constexpr int kIntakeRampStored = 0;

public:
    Servo UpperIntakeServo;
    Servo LowerIntakeServo;
    Servo IntakeRampServo;

    Intake();

    void begin() override;
    void update() override;
    void setState(int state) override;

    void setServo1Position(int position);
    void setServo2Position(int position);
    void setServo3Position(int position);
    
    int getServo1Position() const;
    int getServo2Position() const;
    int getServo3Position() const;

    void setServoPosition(Servo &servo, int position);
    
    enum class IntakeState {
        ALL_SERVOS_STORED = 0,
        ALL_SERVOS_POSITIONED = 1,
        UPPER_SERVO_POSITIONED = 2,
        UPPER_SERVO_AWAY = 3,
        LOWER_SERVO_POSITIONED = 4,
        LOWER_SERVO_AWAY = 5
    };

    IntakeState intake_state_ = IntakeState::ALL_SERVOS_STORED;
};

#endif // INTAKE_H