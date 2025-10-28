#include "Intake.h"

Intake::Intake()
    : UpperIntakeServo(),
      LowerIntakeServo(),
      IntakeRampEnableServo()
{
}

void Intake::begin()
{
    UpperIntakeServo.attach(Pins::kUpperIntakeServo);
    LowerIntakeServo.attach(Pins::kLowerIntakeServo);
    IntakeRampEnableServo.attach(Pins::kIntakeRampEnable);

    setIntakeServoPosition(UpperIntakeServo, IntakeConstants::kAvoidBallUpperServoPosition);
    setIntakeServoPosition(LowerIntakeServo, IntakeConstants::kAvoidBallLowerServoPosition);
    setIntakeServoPosition(IntakeRampEnableServo, IntakeConstants::kIntakeRampStored);

    setState(0);
}

void Intake::update()
{
    switch (intake_state_)
    {
    case IntakeState::ALL_SERVOS_STORED:
        setIntakeServoPosition(UpperIntakeServo, IntakeConstants::kAvoidBallUpperServoPosition);
        setIntakeServoPosition(LowerIntakeServo, IntakeConstants::kAvoidBallLowerServoPosition);
        setIntakeServoPosition(IntakeRampEnableServo, IntakeConstants::kIntakeRampStored);
        break;
    case IntakeState::ALL_SERVOS_POSITIONED:
        setIntakeServoPosition(UpperIntakeServo, IntakeConstants::kGrabBallUpperServoPosition);
        setIntakeServoPosition(LowerIntakeServo, IntakeConstants::kGrabBallLowerServoPosition);
        setIntakeServoPosition(IntakeRampEnableServo, IntakeConstants::kIntakeRampPositioned);
        break;
    case IntakeState::UPPER_SERVO_ONLY_STORED:
        setIntakeServoPosition(UpperIntakeServo, IntakeConstants::kAvoidBallUpperServoPosition);
        break;
    case IntakeState::LOWER_SERVO_ONLY_STORED:
        setIntakeServoPosition(LowerIntakeServo, IntakeConstants::kAvoidBallLowerServoPosition);
        break;
    case IntakeState::UPPER_SERVO_ONLY_POSITIONED:
        setIntakeServoPosition(UpperIntakeServo, IntakeConstants::kGrabBallUpperServoPosition);
        break;
    case IntakeState::LOWER_SERVO_ONLY_POSITIONED:
        setIntakeServoPosition(LowerIntakeServo, IntakeConstants::kGrabBallLowerServoPosition);
        break;
    default:
        setIntakeServoPosition(UpperIntakeServo, IntakeConstants::kAvoidBallUpperServoPosition);
        setIntakeServoPosition(LowerIntakeServo, IntakeConstants::kAvoidBallLowerServoPosition);
        setIntakeServoPosition(IntakeRampEnableServo, IntakeConstants::kIntakeRampStored);
        break;
    }
}

void Intake::setState(int state)
{
    intake_state_ = static_cast<IntakeState>(state);
}

void Intake::vision()
{
    if (millis() - last_value_received > recive_values_interval) {
        values = com_.getMatrix();
        last_value_received = millis();
    }

    int top = values[0];
    int bottom = values[1];

    bool top_ok = (top != 0) && (top != -1);
    bool bottom_ok = (bottom != 0) && (bottom != -1);

    // Solo actualizamos si cambia el estado del sensor
    if (top_ok) {
        setState(4);
    } else {
        setState(2);
    }

    if (bottom_ok) {
        setState(5);
    } else {
        setState(3);
    } 
}

void Intake::setIntakeServoPosition(Servo &servo, int position)
{
    servo.write(position);
}
