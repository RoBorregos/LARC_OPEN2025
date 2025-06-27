/*
 * @file dcmotor.cpp
 * @author Juan Pablo Gutiérrez
 * @brief DC Motor class
 * @version 0.1
 * @date 2025-03-21
 */

#include "dcmotor.hpp"

DCMotor* DCMotor::instances[4] = {nullptr, nullptr, nullptr, nullptr};

DCMotor::DCMotor(int in1, int in2, int pwm, bool inverted, 
                 int encoder_pin, int encoder_active_state, int instance_num, float diameter)
    : instance_num_(instance_num - 1),
      encoder_pin_(encoder_pin),
      diameter_(diameter) {
    in1_pin_ = in1;
    in2_pin_ = in2;
    pwm_pin_ = pwm;
    pinMode(in1_pin_, OUTPUT);
    pinMode(in2_pin_, OUTPUT);
    pinMode(pwm_pin_, OUTPUT);
    pinMode(encoder_pin_, INPUT_PULLUP);
    encoder_active_state_ = encoder_active_state;
    inverted_ = inverted;
    
    instances[instance_num_] = this;

    setupEncoderInterrupt();
}

void DCMotor::setupEncoderInterrupt()
{
    switch (instance_num_)
    {
    case 0:
        attachInterrupt(digitalPinToInterrupt(encoder_pin_), handleEncoderInterrupt0, CHANGE);
        break;
    case 1:
        attachInterrupt(digitalPinToInterrupt(encoder_pin_), handleEncoderInterrupt1, CHANGE);
        break;
    case 2:
        attachInterrupt(digitalPinToInterrupt(encoder_pin_), handleEncoderInterrupt2, CHANGE);
        break;
    case 3:
        attachInterrupt(digitalPinToInterrupt(encoder_pin_), handleEncoderInterrupt3, CHANGE);
        break;
    }
}

void DCMotor::handleEncoderInterrupt0() { 
    if (instances[0]) {
        if (instances[0]->current_direction_ == Direction::FORWARD) {
            instances[0]->encoder_count_++;
        } else {
            instances[0]->encoder_count_--;
        }
    }
}

void DCMotor::handleEncoderInterrupt1() { 
    if (instances[1]) {
        if (instances[1]->current_direction_ == Direction::FORWARD) {
            instances[1]->encoder_count_++;
        } else {
            instances[1]->encoder_count_--;
        }
    }
}

void DCMotor::handleEncoderInterrupt2() { 
    if (instances[2]) {
        if (instances[2]->current_direction_ == Direction::FORWARD) {
            instances[2]->encoder_count_++;
        } else {
            instances[2]->encoder_count_--;
        }
    }
}

void DCMotor::handleEncoderInterrupt3() { 
    if (instances[3]) {
        if (instances[3]->current_direction_ == Direction::FORWARD) {
            instances[3]->encoder_count_++;
        } else {
            instances[3]->encoder_count_--;
        }
    }
}

void DCMotor::move(int speed, Direction direction) {

    if (inverted_) {
        direction = (direction == Direction::FORWARD) ? Direction::BACKWARD : Direction::FORWARD;
    }

    if (direction == Direction::FORWARD) {
        digitalWrite(in1_pin_, HIGH);
        digitalWrite(in2_pin_, LOW);
    } else {
        digitalWrite(in1_pin_, LOW);
        digitalWrite(in2_pin_, HIGH);
    }

    analogWrite(pwm_pin_, speed);
}

void DCMotor::move(int speed) {
    if (speed > 0) {
        move(speed, Direction::FORWARD);
        current_direction_ = Direction::FORWARD;
    } else if (speed < 0) {
        move(abs(speed), Direction::BACKWARD);
        current_direction_ = Direction::BACKWARD;
    } else {
        stop();
    }
}

void DCMotor::stop() {
    digitalWrite(in1_pin_, LOW);
    digitalWrite(in2_pin_, LOW);
    analogWrite(pwm_pin_, 0);
}

int DCMotor::getEncoderCount() {
    return encoder_count_;
}

double DCMotor::getPositionRotations() {
    return (double) encoder_count_ / rotation_factor_;
}

float DCMotor::getPositionMeters() {
    return encoder_count_ * diameter_ * PI / rotation_factor_;
}