#include "Intake.hpp"

Intake::Intake()
    : UpperIntakeServo(),
      LowerIntakeServo(),
      IntakeUpperRampServo()
{
}

void Intake::begin()
{
    UpperIntakeServo.attach(Pins::kUpperIntakeServo);
    LowerIntakeServo.attach(Pins::kLowerIntakeServo);
    IntakeUpperRampServo.attach(Pins::kIntakeUpperRamp);
}

void Intake::update()
{
    switch (intake_state_)
    {
    case IntakeState::ALL_SERVOS_STORED:
        setIntakeServoPosition(UpperIntakeServo, IntakeConstants::kAvoidBallUpperServoPosition);
        // setIntakeServoPosition(LowerIntakeServo, IntakeConstants::kAvoidBallLowerServoPosition);
        setIntakeServoPosition(IntakeUpperRampServo, IntakeConstants::kIntakeRampStored);
        break;
    case IntakeState::ALL_SERVOS_POSITIONED:
        setIntakeServoPosition(UpperIntakeServo, IntakeConstants::kGrabBallUpperServoPosition);
        // setIntakeServoPosition(LowerIntakeServo, IntakeConstants::kGrabBallLowerServoPosition);
        setIntakeServoPosition(IntakeUpperRampServo, IntakeConstants::kIntakeRampPositioned);
        break;
    }
}

void Intake::setState(int state)
{
    switch (state)
    {
    case 0:
        intake_state_ = IntakeState::ALL_SERVOS_STORED;
        break;
    case 1:
        intake_state_ = IntakeState::ALL_SERVOS_POSITIONED;
        break;
    default:
        intake_state_ = IntakeState::ALL_SERVOS_STORED;
        break;
    }
}

void Intake::setIntakeServoPosition(Servo &servo, int position)
{
    servo.write(position);
}

