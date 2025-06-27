/**
 * @file mecanum_wheel_positions.cpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez
 *  
 * @brief Implementation file for the MecanumWheelPositions class.
 */

#include "mecanum_wheel_positions.hpp"

MecanumWheelPositions::MecanumWheelPositions() 
    : frontLeftMeters(0), frontRightMeters(0), rearLeftMeters(0), rearRightMeters(0) {
}

MecanumWheelPositions::MecanumWheelPositions(float front_left, float front_right, float rear_left, float rear_right)
    : frontLeftMeters(front_left), frontRightMeters(front_right), 
      rearLeftMeters(rear_left), rearRightMeters(rear_right) {
} 