/*
 * @file dcmotor.cpp
 * @author Juan Pablo Gutiérrez
 * @brief DC Motor class
 * @version 0.1
 * @date 2025-03-21
 */

#include "dcmotor.hpp"

DCMotor::DCMotor(int in1, int in2, int pwm, bool inverted,
                 int encoder_pin1, int encoder_pin2, float diameter)
    : encoder_(encoder_pin1, encoder_pin2),
      diameter_(diameter)
{
    in1_pin_ = in1;
    in2_pin_ = in2;
    pwm_pin_ = pwm;
    pinMode(in1_pin_, OUTPUT);
    pinMode(in2_pin_, OUTPUT);
    pinMode(pwm_pin_, OUTPUT);
    inverted_ = inverted;
}

DCMotor::DCMotor(int in1, int in2, int pwm, bool inverted,
                 int encoder_pin1, int encoder_pin2)
    : encoder_(encoder_pin1, encoder_pin2)
{
    in1_pin_ = in1;
    in2_pin_ = in2;
    pwm_pin_ = pwm;
    pinMode(in1_pin_, OUTPUT);
    pinMode(in2_pin_, OUTPUT);
    pinMode(pwm_pin_, OUTPUT);
    inverted_ = inverted;
}



void DCMotor::move(int speed, Direction direction)
{

    if (inverted_)
    {
        direction = (direction == Direction::FORWARD) ? Direction::BACKWARD : Direction::FORWARD;
    }

    if (direction == Direction::FORWARD)
    {
        digitalWrite(in1_pin_, HIGH);
        digitalWrite(in2_pin_, LOW);
    }
    else
    {
        digitalWrite(in1_pin_, LOW);
        digitalWrite(in2_pin_, HIGH);
    }

    analogWrite(pwm_pin_, speed);
}

void DCMotor::move(int speed)
{
    if (speed > 0)
    {
        move(speed, Direction::FORWARD);
        current_direction_ = Direction::FORWARD;
    }
    else if (speed < 0)
    {
        move(abs(speed), Direction::BACKWARD);
        current_direction_ = Direction::BACKWARD;
    }
    else
    {
        stop();
    }
}

void DCMotor::stop()
{
    digitalWrite(in1_pin_, LOW);
    digitalWrite(in2_pin_, LOW);
    analogWrite(pwm_pin_, 0);
}

int DCMotor::getEncoderCount()
{
    return encoder_.read();
}

double DCMotor::getPositionRotations()
{
    return (double)encoder_.read() / rotation_factor_;
}

float DCMotor::getPositionMeters()
{
    return encoder_.read() * diameter_ * PI / rotation_factor_;
}