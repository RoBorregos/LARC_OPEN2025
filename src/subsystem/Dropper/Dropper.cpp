#include "Dropper.hpp"

Dropper::Dropper()
    : dropper_servo_(),
      current_position_(CLOSED_POSITION) {}

void Dropper::begin() {
    dropper_servo_.attach(Pins::kDropperServo);   
}

void Dropper::update() {
    switch (dropper_state_) {
        case DropperState::DROPPER_CLOSED:
            setDropperPosition(CLOSED_POSITION);
            break;
        case DropperState::DROPPER_OPEN:
            setDropperPosition(OPEN_POSITION);
            break;
    }
}

void Dropper::setState(int state) {
    switch (state) {
        case 0:
            dropper_state_ = DropperState::DROPPER_CLOSED;
            break;
        case 1:
            dropper_state_ = DropperState::DROPPER_OPEN;
            break;
        default:
            break;
    }
}

void Dropper::setDropperPosition(int position) {
    dropper_servo_.write(position);
    current_position_ = position;
}

int Dropper::getDropperPosition() {
    return current_position_;
}


