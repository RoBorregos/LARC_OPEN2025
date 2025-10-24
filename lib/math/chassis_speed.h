/**
 * @file chassis_speed.hpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez & Dana E. Torres Estrada
 *  
 * @brief Header file for the ChassisSpeed class, which is the class for the chassis speed.
 */

#ifndef CHASSIS_SPEED_HPP
#define CHASSIS_SPEED_HPP

struct ChassisSpeed {
    float vx;    // Linear velocity in the x-direction (forward)
    float vy;    // Linear velocity in the y-direction (sideways)
    float omega; // Angular velocity (rotation)

    ChassisSpeed() : vx(0), vy(0), omega(0) {}
    ChassisSpeed(float x, float y, float z) : vx(x), vy(y), omega(z) {}
};

#endif