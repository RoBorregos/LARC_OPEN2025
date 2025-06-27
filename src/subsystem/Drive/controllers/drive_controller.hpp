/**
 * @file drive_controller.hpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez
 *  
 * @brief Header file for the DriveController class, which converts the desired chassis speed to the robot's drive speed.
 */

#ifndef DRIVE_CONTROLLER_HPP
#define DRIVE_CONTROLLER_HPP

#include "../lib/math/rotation2d.hpp"
#include "../lib/math/chassis_speed.hpp"
class DriveController {
    public:
        DriveController();
        ChassisSpeed update(Rotation2D current_heading, bool field_relative);
        void acceptInput(float vx, float vy, float omega);
    private:
        float vx_;
        float vy_;
        float omega_;
};

#endif
