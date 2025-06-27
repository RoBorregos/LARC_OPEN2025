/**
 * @file robot_constants.hpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez & Dana E. Torres Estrada
 *  
 * @brief Header file for the RobotConstants class, which is the class for the robot constants.
 */
#ifndef ROBOT_CONSTANTS_HPP
#define ROBOT_CONSTANTS_HPP

class RobotConstants {
    public:
        RobotConstants();   
        RobotConstants(float wheel_base, float wheel_track);
        float wheel_base_;
        float wheel_track_;
};

#endif 