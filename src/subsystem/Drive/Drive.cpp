/*
 * @file drive.cpp
 * @date 2025-03-20
 * @author Juan Pablo Gutiérrez
 * 
 * @brief Drive class, which controls the robot's drive system.
 */

#include "Drive.hpp"

Drive::Drive() : 
    front_left_(DCMotor(Pins::kUpperMotors[0], Pins::kUpperMotors[1], Pins::kPwmPin[0], 
               false, Pins::kEncoders[0], LOW, 1, DriveConstants::kWheelDiameter)),
    front_right_(DCMotor(Pins::kUpperMotors[2], Pins::kUpperMotors[3], Pins::kPwmPin[1], 
                true, Pins::kEncoders[1], LOW, 2, DriveConstants::kWheelDiameter)),
    back_left_(DCMotor(Pins::kLowerMotors[0], Pins::kLowerMotors[1], Pins::kPwmPin[2], 
               false, Pins::kEncoders[2], HIGH, 3, DriveConstants::kWheelDiameter)),
    back_right_(DCMotor(Pins::kLowerMotors[2], Pins::kLowerMotors[3], Pins::kPwmPin[3], 
                true, Pins::kEncoders[3], LOW, 4, DriveConstants::kWheelDiameter)),
    bno_(),
    robot_constants_(0.3, 0.3),
    odometry_(0, 0, Rotation2D(0), &bno_, &robot_constants_),
    drive_controller_(),
    heading_controller_()
{
}

void Drive::update() {
    bno_.update();
    
    ChassisSpeed drive_speed = drive_controller_.update(Rotation2D::fromDegrees(bno_.getYaw()), true);
    switch (drive_state_) {
        case DriveState::HEADING_LOCK:
            {
                drive_speed = drive_controller_.update(Rotation2D::fromDegrees(bno_.getYaw()), false);
                drive_speed.setOmega(heading_controller_.update(Rotation2D::fromDegrees(bno_.getYaw())));
                desired_chassis_speed_ = drive_speed;
            }
            break;
        case DriveState::FIELD_ORIENTED:
            {
                desired_chassis_speed_ = drive_speed;
            }
            break;
        case DriveState::ROBOT_ORIENTED:
            {
                drive_speed = drive_controller_.update(Rotation2D::fromDegrees(bno_.getYaw()), false);
                desired_chassis_speed_ = drive_speed;
            }
            break;
    }

    move(desired_chassis_speed_);

    odometry_.updateWithBNO(front_left_.getEncoderCount(), front_right_.getEncoderCount(), back_left_.getEncoderCount(), back_right_.getEncoderCount());
}

void Drive::setState(int state) {
    drive_state_ = static_cast<DriveState>(state);
}

void Drive::acceptInput(float vx, float vy, float omega) {
    drive_controller_.acceptInput(vx, vy, omega);
}

void Drive::acceptHeadingInput(Rotation2D heading) {
    heading_controller_.setDesiredHeading(heading);
}

void Drive::move(ChassisSpeed chassis_speed) {
    int front_left_speed = chassis_speed.getVx() + chassis_speed.getVy() + chassis_speed.getOmega();
    int front_right_speed = -chassis_speed.getVx() + chassis_speed.getVy() - chassis_speed.getOmega();
    int back_left_speed  = -chassis_speed.getVx() + chassis_speed.getVy() + chassis_speed.getOmega();
    int back_right_speed = chassis_speed.getVx() + chassis_speed.getVy() - chassis_speed.getOmega();
    
    front_left_.move(front_left_speed);
    front_right_.move(front_right_speed);
    back_left_.move(back_left_speed);
    back_right_.move(back_right_speed);
}

Pose2D Drive::getPose() {
    return odometry_.getPose();
}

/* Basic Movement Functions */

void Drive::moveForward(int speed) {
    front_left_.move(speed);
    front_right_.move(speed);
    back_left_.move(speed);
    back_right_.move(speed);
}

void Drive::moveBackward(int speed) {
    front_left_.move(-speed);
    front_right_.move(-speed);
    back_left_.move(-speed);
    back_right_.move(-speed);
}

void Drive::moveLeft(int speed) {
    front_left_.move(-speed);
    front_right_.move(speed);
    back_left_.move(-speed);
    back_right_.move(speed);
}

void Drive::moveRight(int speed) {
    front_left_.move(speed);
    front_right_.move(-speed);
    back_left_.move(speed);
    back_right_.move(-speed);
}