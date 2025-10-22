#include "Intake.hpp"

Intake::Intake()
    : UpperIntakeServo(),
      LowerIntakeServo(),
      IntakeRampServo()
     {}

void Intake::begin() {
    UpperIntakeServo.attach(Pins::kUpperIntakeServo);
    LowerIntakeServo.attach(Pins::kLowerIntakeServo);
    IntakeRampServo.attach(Pins::kIntakeRamp);
}

void Intake::update() {
switch (intake_state_) {
        case IntakeState::ALL_SERVOS_STORED:
            setIntakeServoPosition(UpperIntakeServo, kAvoidBallServoPosition);
            setIntakeServoPosition(LowerIntakeServo, kAvoidBallServoPosition);
            setIntakeServoPosition(IntakeRampServo, kIntakeRampStored);
            break;
        case IntakeState::ALL_SERVOS_POSITIONED:
            setIntakeServoPosition(UpperIntakeServo, kGrabBallServoPosition);
            setIntakeServoPosition(LowerIntakeServo, kGrabBallServoPosition);
            setIntakeServoPosition(IntakeRampServo, kIntakeRampPositioned);
            break;
        case IntakeState::UPPER_SERVO_POSITIONED:
            setIntakeServoPosition(UpperIntakeServo, kGrabBallServoPosition);
            break;
        case IntakeState::UPPER_SERVO_AWAY:
            setIntakeServoPosition(UpperIntakeServo, kAvoidBallServoPosition);
            break;
        case IntakeState::LOWER_SERVO_POSITIONED:
            setIntakeServoPosition(UpperIntakeServo, kAvoidBallServoPosition);
            setIntakeServoPosition(LowerIntakeServo, kGrabBallServoPosition);
            break;
        case IntakeState::LOWER_SERVO_AWAY:
            setIntakeServoPosition(LowerIntakeServo, kAvoidBallServoPosition);
            break;
    }
}

void Intake::setState(int state) {
    switch (state) {
        case 0:
            intake_state_ = IntakeState::ALL_SERVOS_STORED;
            break;
        case 1:
            intake_state_ = IntakeState::ALL_SERVOS_POSITIONED;
            break;
        case 2:
            intake_state_ = IntakeState::UPPER_SERVO_POSITIONED;
            break;
        case 3:
            intake_state_ = IntakeState::UPPER_SERVO_AWAY;
            break;
        case 4:
            intake_state_ = IntakeState::LOWER_SERVO_POSITIONED;
            break;
        case 5:
            intake_state_ = IntakeState::LOWER_SERVO_AWAY;
            break;
        default:
            intake_state_ = IntakeState::ALL_SERVOS_STORED;
            break;
    }
}

void Intake::setIntakeServoPosition(Servo &servo, int position) {
    servo.write(position);
}

int Intake::getIntakeServoPosition(Servo &servo) {
    return servo.read();
}
