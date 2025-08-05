/**
 * @file dcmotor.hpp|
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez & Dana E. Torres Estrada
 *
 * @brief Header file for the DCMotor class, which is the class for the DC motor.
 */

#ifndef DCMOTOR_HPP
#define DCMOTOR_HPP

#include <Arduino.h>
#include <Encoder.h>

class DCMotor
{
public:
    enum class Direction
    {
        FORWARD,
        BACKWARD
    };

    DCMotor(int in1, int in2, int pwm, bool inverted,
            int encoder_pin1, int encoder_pin2, float diameter);

    DCMotor(int in1, int in2, int pwm, bool inverted,
            int encoder_pin1, int encoder_pin2);

    void move(int speed, Direction direction);
    void move(int speed);
    void stop();
    int getEncoderCount();
    double getPositionRotations();
    float getPositionMeters();

private:
    int in1_pin_;
    int in2_pin_;
    int pwm_pin_;
    bool inverted_;

    Encoder encoder_;

    Direction current_direction_;

    int rotation_factor_ = 473;
    float diameter_;

    static DCMotor *instance_;
};

#endif