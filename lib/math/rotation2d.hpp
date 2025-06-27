/**
 * @file rotation2d.hpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez & Dana E. Torres Estrada
 *  
 * @brief Header file for the Rotation2D class, which is the class for the rotation.
 */

#ifndef ROTATION2D_HPP
#define ROTATION2D_HPP

#include <cmath>

class Rotation2D {
    public:
        Rotation2D();
        Rotation2D(float theta);
        Rotation2D(float x, float y);
        static Rotation2D fromRadians(float radians);
        static Rotation2D fromDegrees(float degrees);
        float getRadians();
        float getDegrees();
        Rotation2D operator+(const Rotation2D& other);
        Rotation2D operator-(const Rotation2D& other);
        Rotation2D operator*(const float& scalar);
        Rotation2D operator/(const float& scalar);

        Rotation2D inverse() const;
        Rotation2D rotateBy(const Rotation2D& other);
        float theta_;
};

#endif