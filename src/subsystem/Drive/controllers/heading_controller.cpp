/**
 * @file heading_controller.cpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez
 *  
 * @brief Implementation file for the HeadingController class, which controls the robot's heading, while still being able to move the robot.
 */

#include "heading_controller.hpp"
#include <Arduino.h>
HeadingController::HeadingController() : pid_(PIDController(DriveConstants::kHeadingControllerKp, DriveConstants::kHeadingControllerKi, DriveConstants::kHeadingControllerKd, -255.0f, 255.0f)) {
    desired_heading_ = Rotation2D();
    pid_.setEnabled(true);
    pid_.setAngleWrapping(true);
}

float HeadingController::update(Rotation2D current_heading) {
    float output = -pid_.update(current_heading.getRadians(), desired_heading_.getRadians());
    return output;
}

void HeadingController::setDesiredHeading(Rotation2D desired_heading) {
    desired_heading_ = desired_heading;
}