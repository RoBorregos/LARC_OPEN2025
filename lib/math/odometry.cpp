/**
 * @file odometry.cpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez
 *  
 * @brief Implementation file for the Odometry class, which is the class for the odometry.
 */

#include "odometry.hpp"
#include <cmath>

Odometry::Odometry(float x, float y, Rotation2D theta, BNO* bno, RobotConstants* robot_constants) 
    : pose_(x, y, theta), bno_(bno), robot_constants_(robot_constants), use_bno_(true),
      previous_wheel_positions_(), previous_angle_(theta) {
}

Pose2D Odometry::getPose() {
    return pose_;
}

void Odometry::setRobotConstants(RobotConstants* robot_constants) {
    robot_constants_ = robot_constants;
}

void Odometry::copyWheelPositions(const MecanumWheelPositions& source, MecanumWheelPositions& destination) {
    destination.frontLeftMeters = source.frontLeftMeters;
    destination.frontRightMeters = source.frontRightMeters;
    destination.rearLeftMeters = source.rearLeftMeters;
    destination.rearRightMeters = source.rearRightMeters;
}

Twist2d Odometry::toTwist2d(const MecanumWheelPositions& start, const MecanumWheelPositions& end) {
    // Calculate wheel deltas
    float fl_delta = end.frontLeftMeters - start.frontLeftMeters;
    float fr_delta = end.frontRightMeters - start.frontRightMeters;
    float rl_delta = end.rearLeftMeters - start.rearLeftMeters;
    float rr_delta = end.rearRightMeters - start.rearRightMeters;

    // Calculate the change in position
    float dx = (fl_delta + fr_delta + rl_delta + rr_delta) / 4.0;
    float dy = (-fl_delta + fr_delta + rl_delta - rr_delta) / 4.0;
    
    // Calculate the change in heading
    float dtheta = (-fl_delta + fr_delta - rl_delta + rr_delta) / 
                   (2.0 * (robot_constants_->wheel_base_ + robot_constants_->wheel_track_));

    return Twist2d(dx, dy, dtheta);
}

void Odometry::update(float front_left_encoder, float front_right_encoder, float rear_left_encoder, float rear_right_encoder) {
    // Create current wheel positions
    MecanumWheelPositions current_positions(
        front_left_encoder,
        front_right_encoder,
        rear_left_encoder,
        rear_right_encoder
    );

    // Calculate twist
    Twist2d twist = toTwist2d(previous_wheel_positions_, current_positions);

    // Update pose
    float current_theta = pose_.getTheta().getRadians();
    float cos_theta = cos(current_theta);
    float sin_theta = sin(current_theta);

    pose_.setX(pose_.getX() + twist.dx * cos_theta - twist.dy * sin_theta);
    pose_.setY(pose_.getY() + twist.dy * cos_theta + twist.dx * sin_theta);
    pose_.setTheta(Rotation2D(fmod(current_theta + twist.dtheta, 2 * M_PI)));

    // Update previous positions
    copyWheelPositions(current_positions, previous_wheel_positions_);
    previous_angle_ = pose_.getTheta();
}

void Odometry::updateWithBNO(float front_left_encoder, float front_right_encoder, float rear_left_encoder, float rear_right_encoder) {
    if (!use_bno_) {
        // If BNO is not available, fall back to regular odometry
        update(front_left_encoder, front_right_encoder, rear_left_encoder, rear_right_encoder);
        return;
    }
    
    // Update the BNO sensor to get the latest orientation data
    bno_->update();
    
    // Create current wheel positions
    MecanumWheelPositions current_positions(
        front_left_encoder,
        front_right_encoder,
        rear_left_encoder,
        rear_right_encoder
    );

    // Calculate twist
    Twist2d twist = toTwist2d(previous_wheel_positions_, current_positions);

    // Get the current heading from the BNO sensor (in degrees)
    float bno_yaw = bno_->getYaw();
    float bno_yaw_rad = bno_yaw * M_PI / 180.0;
    
    // Update twist's dtheta with BNO data
    twist.dtheta = bno_yaw_rad - previous_angle_.getRadians();

    // Update pose
    float current_theta = bno_yaw_rad;
    float cos_theta = cos(current_theta);
    float sin_theta = sin(current_theta);

    pose_.setX(pose_.getX() + twist.dx * cos_theta - twist.dy * sin_theta);
    pose_.setY(pose_.getY() + twist.dy * cos_theta + twist.dx * sin_theta);
    pose_.setTheta(Rotation2D(bno_yaw_rad));

    // Update previous positions
    copyWheelPositions(current_positions, previous_wheel_positions_);
    previous_angle_ = pose_.getTheta();
}