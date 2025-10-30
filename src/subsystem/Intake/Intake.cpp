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

    setState(1);
    update();
}

void Intake::update()
{
    switch (intake_state_)
    {
    case IntakeState::ALL_SERVOS_STORED:
        setIntakeServoPosition(UpperIntakeServo, IntakeConstants::kStoredUpperIntakeServoPosition);
        setIntakeServoPosition(LowerIntakeServo, IntakeConstants::kStoredLowerIntakeServoPosition);
        setIntakeServoPosition(IntakeRampEnableServo, IntakeConstants::kIntakeRampStored);
        break;
    case IntakeState::ALL_SERVOS_POSITIONED:
        setIntakeServoPosition(UpperIntakeServo, IntakeConstants::kGrabBallUpperServoPosition);
        delay(50);
        setIntakeServoPosition(LowerIntakeServo, IntakeConstants::kGrabBallLowerServoPosition);
        setIntakeServoPosition(IntakeRampEnableServo, IntakeConstants::kIntakeRampPositioned);
        break;
    case IntakeState::UPPER_SERVO_AVOID_BALL:
        setIntakeServoPosition(UpperIntakeServo, IntakeConstants::kAvoidBallUpperServoPosition);
        break;
    case IntakeState::LOWER_SERVO_AVOID_BALL:
        setIntakeServoPosition(LowerIntakeServo, IntakeConstants::kAvoidBallLowerServoPosition);
        break;
    case IntakeState::UPPER_SERVO_ONLY_POSITIONED:
        setIntakeServoPosition(UpperIntakeServo, IntakeConstants::kGrabBallUpperServoPosition);
        break;
    case IntakeState::LOWER_SERVO_ONLY_POSITIONED:
        setIntakeServoPosition(LowerIntakeServo, IntakeConstants::kGrabBallLowerServoPosition);
        break;
    default:
        // setIntakeServoPosition(UpperIntakeServo, IntakeConstants::kAvoidBallUpperServoPosition);
        // setIntakeServoPosition(LowerIntakeServo, IntakeConstants::kAvoidBallLowerServoPosition);
        // setIntakeServoPosition(IntakeRampEnableServo, IntakeConstants::kIntakeRampStored);
        break;
    }
}

void Intake::setState(int state)
{
    intake_state_ = static_cast<IntakeState>(state);
}

void Intake::setIntakeServoPosition(Servo &servo, int position)
{
    servo.write(position);
}

void Intake::intakeServoDetach(Servo &servo)
{
    servo.detach();
}
