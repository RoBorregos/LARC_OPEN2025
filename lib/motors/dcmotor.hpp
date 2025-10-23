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
#include "../controllers/PIDController.hpp"
class DCMotor
{
public:
    enum class Direction
    {
        FORWARD,
        BACKWARD
    };

    DCMotor(int in1, int in2, int pwm, bool inverted,
            int encoder_pin1, int encoder_pin2);

    DCMotor(int in1, int in2, int pwm, bool inverted,
            int encoder_pin1, int encoder_pin2, bool encoder_inverted, float kP, float kI, float kD, float kMaxRpm);

    ~DCMotor();

    void begin();
    void move(int speed, Direction direction);
    void moveStableRPM(double target_rpm);
    void move(int speed);
    void stop();
    void brakeStop();
    int getEncoderCount();
    double getCurrentSpeed();
    
    double pidOutput = 0;
private:
    int in1_pin_;
    int in2_pin_;
    int pwm_pin_;
    int encoder_pin1_;
    int encoder_pin2_;
    bool inverted_;

    Encoder *encoder_;
    bool encoder_inverted_;

    Direction current_direction_;

    float max_rpm_;
    PIDController* velocity_controller_;

    double rpm2pwm(const double rpm);

    double currentMillis = 0;
    double prevMillis = 0;
    double prevCount = 0;
    double current_speed_ = 0;
};

#endif