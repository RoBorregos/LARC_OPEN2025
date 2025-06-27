/**
 * @file twist2d.cpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez 
 *  
 * @brief Implementation file for the Twist2d class.
 */

#include "twist2d.hpp"

Twist2d::Twist2d() : dx(0), dy(0), dtheta(0) {
}

Twist2d::Twist2d(float dx, float dy, float dtheta) 
    : dx(dx), dy(dy), dtheta(dtheta) {
} 