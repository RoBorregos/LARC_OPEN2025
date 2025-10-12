#include "Dropper.hpp"

Dropper::Dropper()
    : servo_pin_(Pins::kDropperServoPin),
      current_position_(POSITION_0) {}

void Dropper::begin() {
    servo_.attach(servo_pin_);
    
    setTo0();
    
}

void Dropper::update() {
}

void Dropper::setState(int state) {
    switch (state) {
        case 0:
            setTo0();
            break;
        case 1:
            setTo180();
            break;
        default:
            break;
    }
}

void Dropper::setPosition(int position) {
    position = constrain(position, POSITION_0, POSITION_180);
    servo_.write(position);
    current_position_ = position;
    
    if (position == POSITION_0) {
        dropper_state_ = DropperState::POSITION_0;
    } else if (position == POSITION_180) {
        dropper_state_ = DropperState::POSITION_180;
    }
    
    delay(15); 
}

void Dropper::setTo0() {
    setPosition(POSITION_0);
    dropper_state_ = DropperState::POSITION_0;
}

void Dropper::setTo180() {
    setPosition(POSITION_180);
    dropper_state_ = DropperState::POSITION_180;
}

int Dropper::getCurrentPosition() const {
    return current_position_;
}

bool Dropper::isAt0() const {
    return current_position_ == POSITION_0;
}

bool Dropper::isAt180() const {
    return current_position_ == POSITION_180;
}
