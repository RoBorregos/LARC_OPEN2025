/*
 * @file drive.cpp
 * @date 2025-03-20
 * @author Juan Pablo Gutiérrez
 *
 * @brief Drive class, which controls the robot's drive system.
 */

#include "Drive.hpp"
#include "../../robot/robot_instances.h"

Drive::Drive() : front_left_(Pins::kUpperMotors[0], Pins::kUpperMotors[1], Pins::kPwmPin[0], true, Pins::kEncoders[0], Pins::kEncoders[1], false, 2.0, 0.35, 0.0025, 0.708333333 * 1.5, 360),
                 front_right_(Pins::kUpperMotors[2], Pins::kUpperMotors[3], Pins::kPwmPin[1], false, Pins::kEncoders[2], Pins::kEncoders[3], false, 2.0,0.35, 0.0025, 0.708333333 * 1.5, 360),
                 back_left_(Pins::kLowerMotors[0], Pins::kLowerMotors[1], Pins::kPwmPin[2], true, Pins::kEncoders[4], Pins::kEncoders[5], false, 2.0, 0.35, 0.0025, 0.708333333 * 1.5, 360),
                 back_right_(Pins::kLowerMotors[2], Pins::kLowerMotors[3], Pins::kPwmPin[3], false, Pins::kEncoders[6], Pins::kEncoders[7], true, 0.2, 0.1, 0.0, 0.708333333 * 0.7, 360),

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

    // move(drive_speed);
    // ChassisSpeed drive_speed;
    //     switch (drive_state_)
    //     {
    //     case DriveState::HEADING_LOCK:
    //     {
    //         drive_speed = drive_controller_.update(Rotation2D::fromDegrees(bno_.getYaw()), false);
    //         drive_speed.omega = heading_controller_.update(Rotation2D::fromDegrees(bno_.getYaw()));
    //     }
    //     break;
    //     case DriveState::FIELD_ORIENTED:
    //     {
    //     }
    //     break;
    //     case DriveState::ROBOT_ORIENTED:
    //     {
    //         drive_speed = drive_controller_.update(Rotation2D::fromDegrees(bno_.getYaw()), false);
    //     }
    //     break;
    // }
    drive_speed = drive_controller_.update(Rotation2D::fromDegrees(bno_.getYaw()), true);

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
    double trackWidth = 0.382;
    double wheelBase = 0.415;

    double fl = chassis_speed.vx + chassis_speed.vy + chassis_speed.omega * (wheelBase + trackWidth);
    double fr = -chassis_speed.vx + chassis_speed.vy - chassis_speed.omega * (wheelBase + trackWidth);
    double bl = -chassis_speed.vx + chassis_speed.vy + chassis_speed.omega * (wheelBase + trackWidth);
    double br = chassis_speed.vx + chassis_speed.vy - chassis_speed.omega * (wheelBase + trackWidth);

    double fl_rpm = fl * 60.0 / DriveConstants::kWheelCircumference;
    double fr_rpm = fr * 60.0 / DriveConstants::kWheelCircumference;
    double bl_rpm = bl * 60.0 / DriveConstants::kWheelCircumference;
    double br_rpm = br * 60.0 / DriveConstants::kWheelCircumference;

    monitor_.print("FL Calculated speed: ");
    monitor_.println(fl);
    monitor_.print("FL Desired RPM: ");
    monitor_.println(fl_rpm);
    monitor_.print("FL Current Speed RPM: ");
    monitor_.println(front_left_.getCurrentSpeed());
    monitor_.print("FL PID Output: ");
    monitor_.println(front_left_.pidOutput);
    monitor_.print("FL KF Output: ");
    monitor_.println(front_left_.kfOutput);

    monitor_.print("FR Calculated speed: ");
    monitor_.println(fr);
    monitor_.print("FR Desired RPM: ");
    monitor_.println(fr_rpm);
    monitor_.print("FR Current Speed RPM: ");
    monitor_.println(front_right_.getCurrentSpeed());
    monitor_.print("FR PID Output: ");
    monitor_.println(front_right_.pidOutput);
    monitor_.print("FR KF Output: ");
    monitor_.println(front_right_.kfOutput);

    monitor_.print("BL Calculated speed: ");
    monitor_.println(bl);
    monitor_.print("BL Desired RPM: ");
    monitor_.println(bl_rpm);
    monitor_.print("BL Current Speed RPM: ");
    monitor_.println(back_left_.getCurrentSpeed());
    monitor_.print("BL PID Output: ");
    monitor_.println(back_left_.pidOutput);
    monitor_.print("BL KF Output: ");
    monitor_.println(back_left_.kfOutput);

    monitor_.print("BR Calculated speed: ");
    monitor_.println(br);
    monitor_.print("BR Desired RPM: ");
    monitor_.println(br_rpm);
    monitor_.print("BR Current Speed RPM: ");
    monitor_.println(back_right_.getCurrentSpeed());
    monitor_.print("BR PID Output: ");
    monitor_.println(back_right_.pidOutput);
    monitor_.print("BR KF Output: ");
    monitor_.println(back_right_.kfOutput);

    front_left_.moveStableRPM(fl_rpm);
    front_right_.moveStableRPM(fr_rpm);
    back_left_.moveStableRPM(bl_rpm);
    back_right_.moveStableRPM(br_rpm);
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

// /* Movement with cm */
// // @brief: This will be used when the robot is align with the endline,
// // the actual error of distance is around 1-2 cm, need a tunning
// void Drive::moveForwardCm(float distance_cm, int speed)
// {
//     resetEncoders();
//     float target_distance = distance_cm / 100.0f;

//     while (getAverageDistanceTraveled() < target_distance)
//     {
//         moveForward(speed);
//         delay(10); // Small delay for stability
//     }
//     brake();
// }

// void Drive::moveBackwardCm(float distance_cm, int speed)
// {
//     resetEncoders();
//     float target_distance = distance_cm / 100.0f;

//     while (getAverageDistanceTraveled() < target_distance)
//     {
//         moveBackward(speed);
//         delay(10);
//     }
//     brake();
// }

// void Drive::moveLeftCm(float distance_cm, int speed)
// {
//     resetEncoders();
//     float target_distance = distance_cm / 100.0f;

//     while (getAverageDistanceTraveled() < target_distance)
//     {
//         moveLeft(speed);
//         delay(10);
//     }
//     brake();
// }

// void Drive::moveRightCm(float distance_cm, int speed)
// {
//     resetEncoders();
//     float target_distance = distance_cm / 100.0f;

//     while (getAverageDistanceTraveled() < target_distance)
//     {
//         moveRight(speed);
//         delay(10);
//     }
//     brake();
// }

void Drive::resetEncoders()
{
    front_left_.getEncoderCount();
    front_right_.getEncoderCount();
    back_left_.getEncoderCount();
    back_right_.getEncoderCount();
}
