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
#include "../math/odometry.hpp"
#include "../math/robot_constants.hpp"
#include "../systems/system.hpp"
#include "../math/chassis_speed.hpp"
#include "controllers/drive_controller.hpp"
#include "controllers/heading_controller.hpp"
#include "constants/constants.h"
using namespace Constants;

class Drive : public System
{
public:
    Drive();
    void moveForward(int speed);
    void moveBackward(int speed);
    void moveLeft(int speed);
    void moveRight(int speed);
    Pose2D getPose();
    void update() override;
    void setState(int state) override;
    void acceptInput(float vx, float vy, float omega);
    void acceptHeadingInput(Rotation2D heading);
private:
    void move(ChassisSpeed chassis_speed);
    void moveXYOmega(ChassisSpeed chassis_speed);
    DCMotor front_left_;
    DCMotor front_right_;
    DCMotor back_left_;
    DCMotor back_right_;
    BNO bno_;
    RobotConstants robot_constants_;
    Odometry odometry_;

    enum class DriveState {
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
};

#endif
