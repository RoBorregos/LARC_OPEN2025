/**
 * @file mecanum_wheel_positions.hpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez
 *  
 * @brief Header file for the MecanumWheelPositions class, which stores the positions of the mecanum drive wheels.
 */

#ifndef MECANUM_WHEEL_POSITIONS_HPP
#define MECANUM_WHEEL_POSITIONS_HPP

class MecanumWheelPositions {
    public:
        MecanumWheelPositions();
        MecanumWheelPositions(float front_left, float front_right, float rear_left, float rear_right);
        float frontLeftMeters;
        float frontRightMeters;
        float rearLeftMeters;
        float rearRightMeters;
};

#endif 