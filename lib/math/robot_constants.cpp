/**
 * @file robot_constants.cpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez & Dana E. Torres Estrada
 *  
 * @brief Implementation file for the RobotConstants class, which is the class for the robot constants.
 */

#include "robot_constants.hpp"

RobotConstants::RobotConstants() : wheel_base_(0.3), wheel_track_(0.3) {
}

RobotConstants::RobotConstants(float wheel_base, float wheel_track) : wheel_base_(wheel_base), wheel_track_(wheel_track) {
} 