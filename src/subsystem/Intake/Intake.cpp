#include "Intake.hpp"

Sorter::Sorter()
    : servo1_pin_(Pins::kSorterServo1Pin),
      servo2_pin_(Pins::kSorterServo2Pin),
      servo3_pin_(Pins::kSorterServo3Pin),
      servo1_position_(POSITION_0),
      servo2_position_(POSITION_0),
      servo3_position_(POSITION_0) {}

void Sorter::begin() {
    servo1_.attach(servo1_pin_);
    servo2_.attach(servo2_pin_);
    servo3_.attach(servo3_pin_);
    
    setAllServosTo0();    
}

void Sorter::update() {
}

void Sorter::setState(int state) {
    switch (state) {
        case 0:
            setAllServosTo0();
            sorter_state_ = SorterState::ALL_0;
            break;
        case 1:
            setAllServosTo180();
            sorter_state_ = SorterState::ALL_180;
            break;
        default:
            sorter_state_ = SorterState::CUSTOM;
            break;
    }
}

void Sorter::setServo1Position(int position) {
    position = constrain(position, POSITION_0, POSITION_180);
    servo1_.write(position);
    servo1_position_ = position;
    delay(15); 
}

void Sorter::setServo2Position(int position) {
    position = constrain(position, POSITION_0, POSITION_180);
    servo2_.write(position);
    servo2_position_ = position;
    delay(15); 
}

void Sorter::setServo3Position(int position) {
    position = constrain(position, POSITION_0, POSITION_180);
    servo3_.write(position);
    servo3_position_ = position;
    delay(15); 
}

void Sorter::setServo1To0() {
    setServo1Position(POSITION_0);
}

void Sorter::setServo1To180() {
    setServo1Position(POSITION_180);
}

void Sorter::setServo2To0() {
    setServo2Position(POSITION_0);
}

void Sorter::setServo2To180() {
    setServo2Position(POSITION_180);
}

void Sorter::setServo3To0() {
    setServo3Position(POSITION_0);
}

void Sorter::setServo3To180() {
    setServo3Position(POSITION_180);
}

int Sorter::getServo1Position() const {
    return servo1_position_;
}

int Sorter::getServo2Position() const {
    return servo2_position_;
}

int Sorter::getServo3Position() const {
    return servo3_position_;
}

void Sorter::setAllServosTo0() {
    setServo1To0();
    setServo2To0();
    setServo3To0();
    sorter_state_ = SorterState::ALL_0;
}

void Sorter::setAllServosTo180() {
    setServo1To180();
    setServo2To180();
    setServo3To180();
    sorter_state_ = SorterState::ALL_180;
}

void Sorter::setAllServosToPosition(int position) {
    setServo1Position(position);
    setServo2Position(position);
    setServo3Position(position);
    sorter_state_ = SorterState::CUSTOM;
}