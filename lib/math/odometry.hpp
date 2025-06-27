/**
 * @file odometry.hpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez 
 *  
 * @brief Header file for the Odometry class, which is the class for the odometry.
 */

#ifndef ODOMETRY_HPP
#define ODOMETRY_HPP

#include "pose2d.hpp"
#include "../sensors/bno.hpp"
#include "robot_constants.hpp"
#include "mecanum_wheel_positions.hpp"
#include "twist2d.hpp"

class Odometry {
    public:
        Odometry(float x, float y, Rotation2D theta, BNO* bno, RobotConstants* robot_constants);
        void setRobotConstants(RobotConstants* robot_constants);
        Pose2D getPose();
        void update(float front_left_encoder, float front_right_encoder, float rear_left_encoder, float rear_right_encoder);
        void updateWithBNO(float front_left_encoder, float front_right_encoder, float rear_left_encoder, float rear_right_encoder);
    private:
        Pose2D pose_;
        BNO* bno_;
        RobotConstants* robot_constants_;
        bool use_bno_;
        MecanumWheelPositions previous_wheel_positions_;
        Rotation2D previous_angle_;
        
        Twist2d toTwist2d(const MecanumWheelPositions& start, const MecanumWheelPositions& end);
        void copyWheelPositions(const MecanumWheelPositions& source, MecanumWheelPositions& destination);
};

#endif