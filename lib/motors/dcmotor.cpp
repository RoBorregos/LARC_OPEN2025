/*
 * @file dcmotor.cpp
 * @author Juan Pablo Gutiérrez
 * @brief DC Motor class
 * @version 0.1
 * @date 2025-03-21
 */

#include "dcmotor.hpp"
#include <math.h>

DCMotor::DCMotor(int in1, int in2, int pwm, bool inverted,
                 int encoder_pin1, int encoder_pin2)
{
    in1_pin_ = in1;
    in2_pin_ = in2;
    pwm_pin_ = pwm;
    encoder_pin1_ = encoder_pin1;
    encoder_pin2_ = encoder_pin2;

    inverted_ = inverted;
}

DCMotor::DCMotor(int in1, int in2, int pwm, bool inverted,
                 int encoder_pin1, int encoder_pin2, bool encoder_inverted, float kP, float kI, float kD, float kf, float kMaxRpm)
{
    in1_pin_ = in1;
    in2_pin_ = in2;
    pwm_pin_ = pwm;
    encoder_pin1_ = encoder_pin1;
    encoder_pin2_ = encoder_pin2;

    inverted_ = inverted;
    encoder_inverted_ = encoder_inverted;

    max_rpm_ = kMaxRpm;
    velocity_controller_ = new PIDController(kP, kI, kD, -255.0f, 255.0f);
    kf_ = kf;
}

DCMotor::~DCMotor()
{
    if (encoder_ != nullptr)
    {
        delete encoder_;
        encoder_ = nullptr;
    }
}

void DCMotor::begin()
{
    pinMode(in1_pin_, OUTPUT);
    pinMode(in2_pin_, OUTPUT);
    pinMode(pwm_pin_, OUTPUT);
    pinMode(encoder_pin1_, INPUT_PULLUP);
    pinMode(encoder_pin2_, INPUT_PULLUP);
    encoder_ = new Encoder(encoder_pin1_, encoder_pin2_);

    if (velocity_controller_ != nullptr)
    {
        velocity_controller_->setEnabled(true);
        velocity_controller_->reset();
    }
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

void DCMotor::moveStableRPM(double target_rpm)
{
    if (velocity_controller_ == nullptr)
    {
        return;
    }
    double current_speed = getCurrentSpeed();

    if (abs(current_speed) < 10e-5 && abs(target_rpm) < 10e-5){
        velocity_controller_->reset();
    }

    auto tmp_pwm = -velocity_controller_->update(target_rpm, current_speed);

    kfOutput = kf_ * target_rpm;
    pidOutput = (int)tmp_pwm + kfOutput;

    double pwm = rpm2pwm(pidOutput);

    move((int)pwm);
}

double DCMotor::getCurrentSpeed()
{
    unsigned long currentMillis = millis();

    if ((currentMillis - prevMillis) >= 100)
    {
        long newCount = encoder_->read();
        long deltaCount = newCount - prevCount;

        float deltaTime = (currentMillis - prevMillis) / 60000.0;

        current_speed_ = (deltaCount / 1920.0) / deltaTime;

        prevCount = newCount;
        prevMillis = currentMillis;
    }

    if (encoder_inverted_)
    {
        current_speed_ = -current_speed_;
    }

    return current_speed_;
}

double DCMotor::rpm2pwm(const double rpm)
{
    return rpm * (255.0 / max_rpm_);
}

void DCMotor::stop()
{
    digitalWrite(in1_pin_, LOW);
    digitalWrite(in2_pin_, LOW);
    analogWrite(pwm_pin_, 0);
}

void DCMotor::brakeStop()
{
    digitalWrite(in1_pin_, HIGH);
    digitalWrite(in2_pin_, HIGH);
    analogWrite(pwm_pin_, 0);
}

int DCMotor::getEncoderCount()
{
    return encoder_ != nullptr ? encoder_->read() : 0;
}
