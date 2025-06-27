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

class DCMotor
{
public:
    enum class Direction
    {
        FORWARD,
        BACKWARD
    };

    DCMotor(int in1, int in2, int pwm, bool inverted,
            int encoder_pin, int encoder_active_state, int instance_num, float diameter);

    void move(int speed, Direction direction);
    void move(int speed);
    void stop();
    int getEncoderCount();
    double getPositionRotations();
    float getPositionMeters();
    void setupEncoderInterrupt();

private:
    int in1_pin_;
    int in2_pin_;
    int pwm_pin_;
    bool inverted_;

    static DCMotor *instances[4]; // Array for up to 4 motors
    int instance_num_;
    volatile int encoder_count_ = 0;
    int encoder_pin_;
    Direction current_direction_;   

    static void handleEncoderInterrupt0();
    static void handleEncoderInterrupt1();
    static void handleEncoderInterrupt2();
    static void handleEncoderInterrupt3();

    volatile int encoder_active_state_;
    int rotation_factor_ = 473;
    float diameter_;

    static DCMotor *instance_;
};

#endif