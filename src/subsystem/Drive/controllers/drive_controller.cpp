/**
 * @file drive_controller.cpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez
 *  
 * @brief Implementation file for the DriveController class, which converts the desired chassis speed to the robot's drive speed.
 */

#include "drive_controller.hpp"
#include <cmath>

DriveController::DriveController() {
    vx_ = 0.0f;
    vy_ = 0.0f;
    omega_ = 0.0f;
}

ChassisSpeed DriveController::update(Rotation2D current_heading, bool field_relative) { 
    if (field_relative) {
        float sin_theta = sin(current_heading.getRadians());
        float cos_theta = cos(current_heading.getRadians());
        
        float robot_vx = vx_ * cos_theta + vy_ * sin_theta;
        float robot_vy = -vx_ * sin_theta + vy_ * cos_theta;
        
        return ChassisSpeed(robot_vx, robot_vy, omega_);
    } else {
        return ChassisSpeed(vx_, vy_, omega_);
    }
}

void DriveController::acceptInput(float vx, float vy, float omega) {
    vx_ = vx;
    vy_ = vy;
    omega_ = omega;
}
