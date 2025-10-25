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
    switch (state)
    {
    case 0:
        intake_state_ = IntakeState::ALL_SERVOS_STORED;
        break;
    case 1:
        intake_state_ = IntakeState::ALL_SERVOS_POSITIONED;
        break;
    case 2:
        intake_state_ = IntakeState::UPPER_SERVO_ONLY_STORED;
        break;
    case 3:

        intake_state_ = IntakeState::LOWER_SERVO_ONLY_STORED;
        break;
    case 4:
        intake_state_ = IntakeState::UPPER_SERVO_ONLY_POSITIONED;
        break;
    case 5:
        intake_state_ = IntakeState::LOWER_SERVO_ONLY_POSITIONED;
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
