/*
 * @file drive.cpp
 * @date 2025-03-20
 * @author Juan Pablo Gutiérrez
 *
 * @brief Drive class, which controls the robot's drive system.
 */

#include "Drive.h"

Drive::Drive() : front_left_(Pins::kUpperMotors[0], Pins::kUpperMotors[1], Pins::kPwmPin[0], true, Pins::kEncoders[0], Pins::kEncoders[1], DriveConstants::kWheelDiameter),
                 front_right_(Pins::kUpperMotors[2], Pins::kUpperMotors[3], Pins::kPwmPin[1], false, Pins::kEncoders[2], Pins::kEncoders[3], DriveConstants::kWheelDiameter),
                 back_left_(Pins::kLowerMotors[0], Pins::kLowerMotors[1], Pins::kPwmPin[2], true, Pins::kEncoders[4], Pins::kEncoders[5], DriveConstants::kWheelDiameter),
                 back_right_(Pins::kLowerMotors[2], Pins::kLowerMotors[3], Pins::kPwmPin[3], false, Pins::kEncoders[6], Pins::kEncoders[7], DriveConstants::kWheelDiameter),
                 bno_(),
                 drive_controller_(),
                 heading_controller_()
{
}

void Drive::begin()
{
    bno_.begin();
    front_left_.begin();
    front_right_.begin();
    back_left_.begin();
    back_right_.begin();
}

void Drive::update()
{
    bno_.update();

    ChassisSpeed drive_speed;
    switch (drive_state_)
    {
    case DriveState::HEADING_LOCK:
    {
        drive_speed = drive_controller_.update(Rotation2D::fromDegrees(bno_.getYaw()), false);
        drive_speed.omega = heading_controller_.update(Rotation2D::fromDegrees(bno_.getYaw()));
    }
    break;
    case DriveState::FIELD_ORIENTED:
    {
        drive_speed = drive_controller_.update(Rotation2D::fromDegrees(bno_.getYaw()), true);
    }
    break;
    case DriveState::ROBOT_ORIENTED:
    {
        drive_speed = drive_controller_.update(Rotation2D::fromDegrees(bno_.getYaw()), false);
    }
    break;
    }

    move(drive_speed);
}

void Drive::setState(int state)
{
    drive_state_ = static_cast<DriveState>(state);
}

void Drive::acceptInput(float vx, float vy, float omega)
{
    drive_controller_.acceptInput(vx, vy, omega);
}

void Drive::acceptHeadingInput(Rotation2D heading)
{
    heading_controller_.setDesiredHeading(heading);
}

void Drive::move(ChassisSpeed chassis_speed)
{
    int front_left_speed = chassis_speed.vx + chassis_speed.vy + chassis_speed.omega;
    int front_right_speed = -chassis_speed.vx + chassis_speed.vy - chassis_speed.omega;
    int back_left_speed = -chassis_speed.vx + chassis_speed.vy + chassis_speed.omega;
    int back_right_speed = chassis_speed.vx + chassis_speed.vy - chassis_speed.omega;

    front_left_.move(front_left_speed);
    front_right_.move(front_right_speed);
    back_left_.move(back_left_speed);
    back_right_.move(back_right_speed);
}

bool Drive::isAtHeadingTarget()
{
    Rotation2D error = getHeadingError();
    float error_degrees = fabs(error.getDegrees());
    return error_degrees < DriveConstants::kHeadingToleranceDegrees;
}

/* Basic Movement Functions */

void Drive::moveForward(int speed)
{
    front_left_.move(speed);
    front_right_.move(speed);
    back_left_.move(speed);
    back_right_.move(speed);
}

void Drive::moveBackward(int speed)
{
    front_left_.move(-speed);
    front_right_.move(-speed);
    back_left_.move(-speed);
    back_right_.move(-speed);
}

void Drive::moveLeft(int speed)
{
    front_left_.move(-speed);
    front_right_.move(speed);
    back_left_.move(speed);
    back_right_.move(-speed);
}

void Drive::moveRight(int speed)
{
    front_left_.move(speed);
    front_right_.move(-speed);
    back_left_.move(-speed);
    back_right_.move(speed);
}

void Drive::brake()
{
    front_left_.stop();
    front_right_.stop();
    back_left_.stop();
    back_right_.stop();
}

void Drive::hardBrake()
{
    front_left_.brakeStop();
    front_right_.brakeStop();
    back_left_.brakeStop();
    back_right_.brakeStop();
}

float Drive::getYaw()
{
    return bno_.getYaw();
}

Rotation2D Drive::getHeadingError()
{
    return heading_controller_.getError(Rotation2D::fromDegrees(bno_.getYaw()));
}

void Drive::motorTest()
{
    front_left_.move(100);
    delay(3000);
    front_left_.move(0);
    delay(3000);

    front_left_.move(-100);
    delay(3000);
    front_left_.move(0);
    delay(3000);

    front_right_.move(100);
    delay(3000);
    front_right_.move(0);
    delay(3000);

    front_right_.move(-100);
    delay(3000);
    front_right_.move(0);
    delay(3000);

    back_left_.move(100);
    delay(3000);
    back_left_.move(0);
    delay(3000);

    back_left_.move(-100);
    delay(3000);
    back_left_.move(0);
    delay(3000);

    back_right_.move(100);
    delay(3000);
    back_right_.move(0);
    delay(3000);

    back_right_.move(-100);
    delay(3000);
    back_right_.move(0);
    delay(3000);
}
