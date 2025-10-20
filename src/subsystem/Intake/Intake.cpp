#include "Intake.hpp"

Intake::Intake()
    : servo1_pin_(-1),
      servo2_pin_(-1),
      servo3_pin_(-1),
      servo1_position_(POSITION_0),
      servo2_position_(POSITION_0),
      servo3_position_(POSITION_0) {}

void Intake::begin() {
    servo1_.attach(servo1_pin_);
    servo2_.attach(servo2_pin_);
    servo3_.attach(servo3_pin_);
    
    setAllServosTo0();    
}

void Intake::update() {
}

void Intake::setState(int state) {
    switch (state) {
        case 0:
            setAllServosTo0();
            intake_state_ = IntakeState::ALL_0;
            break;
        case 1:
            setAllServosTo180();
            intake_state_ = IntakeState::ALL_180;
            break;
        default:
            intake_state_ = IntakeState::CUSTOM;
            break;
    }
}

void Intake::setServo1Position(int position) {
    position = constrain(position, POSITION_0, POSITION_180);
    servo1_.write(position);
    servo1_position_ = position;
    delay(15); 
}

void Intake::setServo2Position(int position) {
    position = constrain(position, POSITION_0, POSITION_180);
    servo2_.write(position);
    servo2_position_ = position;
    delay(15); 
}

void Intake::setServo3Position(int position) {
    position = constrain(position, POSITION_0, POSITION_180);
    servo3_.write(position);
    servo3_position_ = position;
    delay(15); 
}

void Intake::setServo1To0() {
    setServo1Position(POSITION_0);
}

void Intake::setServo1To180() {
    setServo1Position(POSITION_180);
}

void Intake::setServo2To0() {
    setServo2Position(POSITION_0);
}

void Intake::setServo2To180() {
    setServo2Position(POSITION_180);
}

void Intake::setServo3To0() {
    setServo3Position(POSITION_0);
}

void Intake::setServo3To180() {
    setServo3Position(POSITION_180);
}

int Intake::getServo1Position() const {
    return servo1_position_;
}

int Intake::getServo2Position() const {
    return servo2_position_;
}

int Intake::getServo3Position() const {
    return servo3_position_;
}

void Intake::setAllServosTo0() {
    setServo1To0();
    setServo2To0();
    setServo3To0();
    intake_state_ = IntakeState::ALL_0;
}

void Intake::setAllServosTo180() {
    setServo1To180();
    setServo2To180();
    setServo3To180();
    intake_state_ = IntakeState::ALL_180;
}

void Intake::setAllServosToPosition(int position) {
    setServo1Position(position);
    setServo2Position(position);
    setServo3Position(position);
    intake_state_ = IntakeState::CUSTOM;
}