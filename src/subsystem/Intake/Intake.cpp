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
            setServoPosition(UpperIntakeServo, kAvoidBallServoPosition);
            setServoPosition(LowerIntakeServo, kAvoidBallServoPosition);
            setServoPosition(IntakeRampServo, kIntakeRampStored);
            break;
        case IntakeState::ALL_SERVOS_POSITIONED:
            setServoPosition(UpperIntakeServo, kGrabBallServoPosition);
            setServoPosition(LowerIntakeServo, kGrabBallServoPosition);
            setServoPosition(IntakeRampServo, kIntakeRampPositioned);
            break;
        case IntakeState::UPPER_SERVO_POSITIONED:
            setServoPosition(UpperIntakeServo, kGrabBallServoPosition);
            break;
        case IntakeState::UPPER_SERVO_AWAY:
            setServoPosition(UpperIntakeServo, kAvoidBallServoPosition);
            break;
        case IntakeState::LOWER_SERVO_POSITIONED:
            setServoPosition(UpperIntakeServo, kAvoidBallServoPosition);
            setServoPosition(LowerIntakeServo, kGrabBallServoPosition);
            break;
        case IntakeState::LOWER_SERVO_AWAY:
            setServoPosition(LowerIntakeServo, kAvoidBallServoPosition);
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

void Intake::setServoPosition(Servo &servo, int position) {
    servo.write(position);
}

// void Intake::setServo1Position(int position) {
//     position = constrain(position, POSITION_0, POSITION_180);
//     servo1_.write(position);
//     servo1_position_ = position;
//     delay(15); 
// }

// void Intake::setServo2Position(int position) {
//     position = constrain(position, POSITION_0, POSITION_180);
//     servo2_.write(position);
//     servo2_position_ = position;
//     delay(15); 
// }

// void Intake::setServo3Position(int position) {
//     position = constrain(position, POSITION_0, POSITION_180);
//     servo3_.write(position);
//     servo3_position_ = position;
//     delay(15); 
// }

// int Intake::getServo1Position() const {
//     return servo1_position_;
// }

// int Intake::getServo2Position() const {
//     return servo2_position_;
// }

// int Intake::getServo3Position() const {
//     return servo3_position_;
// }

// void Intake::setServoPosition(Servo &servo, int position) {
//     servo.write(position);
// }

