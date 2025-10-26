/**
 * @file drive.hpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez
 *
 * @brief Header file for the Drive class, which controls the robot's drive system.
 */

#pragma once

#include <Arduino.h>
#include <dcmotor.hpp>
#include "constants/pins.h"
#include "../sensors/bno.hpp"
#include "../systems/system.hpp"
#include "../math/chassis_speed.h"
#include "controllers/heading_controller.hpp"
#include "controllers/drive_controller.hpp"
#include "constants/constants.h"

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

    float getYaw();
    Rotation2D getHeadingError();
    bool isAtHeadingTarget();

    /* Debugging functions */
    void moveForward(int speed);
    void moveBackward(int speed);
    void moveLeft(int speed);
    void moveRight(int speed);
    void motorTest();

private:
    void move(ChassisSpeed chassis_speed);
    DCMotor front_left_;
    DCMotor front_right_;
    DCMotor back_left_;
    DCMotor back_right_;
    BNO bno_;

    enum class DriveState
    {
        HEADING_LOCK = 0,
        FIELD_ORIENTED = 1,
        ROBOT_ORIENTED = 2,
    };

    DriveState drive_state_ = DriveState::FIELD_ORIENTED;

    /* Drive controllers */
    DriveController drive_controller_;
    HeadingController heading_controller_;
};