/**
 * @file twist2d.hpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez 
 *  
 * @brief Header file for the Twist2d class, which represents the robot's movement.
 */

#ifndef TWIST2D_HPP
#define TWIST2D_HPP

class Twist2d {
    public:
        Twist2d();
        Twist2d(float dx, float dy, float dtheta);
        float dx;
        float dy;
        float dtheta;
};

#endif 