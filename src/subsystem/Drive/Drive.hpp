/**
 * @file drive.hpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez
 *
 * @brief Header file for the Drive class, which controls the robot's drive system.
 */

#ifndef DRIVE_HPP
#define DRIVE_HPP
#include <Arduino.h>
#include <dcmotor.hpp>
#include "constants/pins.h"
#include "../sensors/bno.hpp"
#include "../math/robot_constants.hpp"
#include "../systems/system.hpp"
#include "../math/chassis_speed.hpp"
#include "controllers/drive_controller.hpp"
#include "controllers/heading_controller.hpp"
#include "constants/constants.h"
#include "../LineSensor/LineSensor.hpp"
using namespace Constants;

class Drive : public System
{
public:
    Drive();
    void begin() override;
    void update() override;
    void setState(int state) override;
    void acceptInput(float vx, float vy, float omega);
    void acceptHeadingInput(Rotation2D heading);
    void brake();
    void hardBrake();

    void moveForwardCm(float distance_cm, int speed = 100);
    void moveBackwardCm(float distance_cm, int speed = 100);
    void moveLeftCm(float distance_cm, int speed = 100);
    void moveRightCm(float distance_cm, int speed = 100);
    void resetEncoders();
    float getAverageDistanceTraveled();

    void followFrontLine(int movement);

    void setLinePIDConstants(float kp, float ki, float kd);
    float calculateLineError(const std::vector<int> &sensors);
    float calculateLinePID();

    /* Debugging functions */
    void moveForward(int speed);
    void moveBackward(int speed);
    void moveLeft(int speed);
    void moveRight(int speed);
    void motorTest();

private:
    void move(ChassisSpeed chassis_speed);
    void moveXYOmega(ChassisSpeed chassis_speed);
    DCMotor front_left_;
    DCMotor front_right_;
    DCMotor back_left_;
    DCMotor back_right_;
    BNO bno_;
    RobotConstants robot_constants_;
    LineSensor line_sensor_;

    enum class DriveState
    {
        HEADING_LOCK = 0,
        FIELD_ORIENTED = 1,
        ROBOT_ORIENTED = 2,
    };

    ChassisSpeed desired_chassis_speed_;
    float heading_lock_theta_;

    DriveState drive_state_ = DriveState::FIELD_ORIENTED;

    /* Drive controllers */
    DriveController drive_controller_;
    HeadingController heading_controller_;

    /* Line following PID variables */
    float line_kp_ = 0.4f;
    float line_ki_ = 0.02f;
    float line_kd_ = 0.1f;
    float line_error_ = 0.0f;
    float line_last_error_ = 0.0f;
    float line_integral_ = 0.0f;
    unsigned long line_last_time_ = 0;
};

#endif
