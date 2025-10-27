#include "Sorter.hpp"

Sorter::Sorter()
    : stepper{kMotorInterfaceType, Pins::kSorterStepPin, Pins::kSorterDirPin}
       {}

void Sorter::begin() {
    stepper.setMaxSpeed(kMaxStepperSpeed);
    stepper.setAcceleration(kStepperAcceleration);
    stepper.setCurrentPosition(0);
}
    
void Sorter::update() {
    stepper.runToPosition();
}

void Sorter::setState(int state) {
    moveToState(static_cast<Sorter::SorterState>(state));
}

void Sorter::moveToState(Sorter::SorterState target_state) {
    int target_index = static_cast<int>(target_state);
    long target_steps = static_cast<long>(target_index) * static_cast<long>(Sorter::kStepsPerPosition);
    // schedule movement
    stepper.move(target_steps);
    // store desired logical state (note: this is the requested state; the physical position will be reached asynchronously)
    sorter_state_ = target_state;
}

bool Sorter::isRunning() {
    return stepper.distanceToGo() != 0;
} 

void Sorter::setMaxSpeed(uint16_t speed) {
    stepper.setMaxSpeed(speed);
}

void Sorter::setAcceleration(uint16_t acceleration) {
    stepper.setAcceleration(acceleration);
}

void Sorter::setSpeed(uint16_t speed) {
    // setSpeed sets the current speed (steps/s) for runSpeed/runSpeedToPosition usage
    stepper.setSpeed(speed);
}

// void Sorter::update() {
//     if (is_running_) {
//         unsigned long current_time = micros();
//         if (current_time - last_step_time_ >= step_delay_us_) {
//             if (current_position_ != target_position_) {
//                 performStep();
//                 last_step_time_ = current_time;
//             } else {
//                 is_running_ = false;
//             }
//         }
//     }
// }

// void Sorter::setState(int state) {
//     switch (state) {
//         case 0:
//             moveToPosition0();
//             break;
//         case 1:
//             moveToPosition180();
//             break;
//         default:
//             sorter_state_ = SorterState::CUSTOM;
//             break;
//     }
// }

// void Sorter::startStepper(int target_position, unsigned long speed_us) {
//     target_position_ = target_position;
//     step_delay_us_ = speed_us;
//     is_running_ = true;
//     last_step_time_ = micros();
// }

// void Sorter::stopStepper() {
//     is_running_ = false;
//     digitalWrite(step_pin_, LOW);
// }

// void Sorter::step(bool direction) {
//     setDirection(direction);
//     digitalWrite(step_pin_, HIGH);
//     delayMicroseconds(10); 
//     digitalWrite(step_pin_, LOW);
//     delayMicroseconds(step_delay_us_);
    
//     current_position_ += direction ? 1 : -1;
// }

// bool Sorter::isRunning() const {
//     return is_running_;
// }

// void Sorter::moveToPosition0() {
//     startStepper(POSITION_0);
//     sorter_state_ = SorterState::POSITION_0;
// }

// void Sorter::moveToPosition180() {
//     startStepper(POSITION_180);
//     sorter_state_ = SorterState::POSITION_180;
// }

// void Sorter::moveToPosition(int position) {
//     startStepper(position);
//     sorter_state_ = SorterState::CUSTOM;
// }

// int Sorter::getCurrentPosition() const {
//     return current_position_;
// }

// void Sorter::setDirection(bool forward) {
//     digitalWrite(dir_pin_, forward ? HIGH : LOW);
//     delayMicroseconds(5); 
// }

// void Sorter::performStep() {
//     bool direction = target_position_ > current_position_;
//     setDirection(direction);
    
//     digitalWrite(step_pin_, HIGH);
//     delayMicroseconds(10);
//     digitalWrite(step_pin_, LOW);
    
//     current_position_ += direction ? 1 : -1;
// }