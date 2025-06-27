/**
 * @file heading_controller.hpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez
 *  
 * @brief Header file for the HeadingController class, which controls the robot's heading, while still being able to move the robot.
 */

#ifndef HEADING_CONTROLLER_HPP
#define HEADING_CONTROLLER_HPP

#include "../lib/math/rotation2d.hpp"
#include "../lib/controllers/PIDController.hpp"
#include "constants/constants.h"

using namespace Constants;
class HeadingController {
    public:
        HeadingController();
        float update(Rotation2D current_heading);
        void setDesiredHeading(Rotation2D desired_heading);
    private:
        Rotation2D desired_heading_;
        double input_;
        double output_;
        double setpoint_;
        PIDController pid_;
};

#endif
