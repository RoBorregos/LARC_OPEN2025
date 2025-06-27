/**
 * @file rotation2d.cpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez & Dana E. Torres Estrada
 *  
 * @brief Implementation file for the Rotation2D class, which is the class for the rotation.
 */

#include "rotation2d.hpp"

Rotation2D::Rotation2D() {
    theta_ = 0.0f;
}

Rotation2D::Rotation2D(float theta) {
    theta_ = theta;
}

Rotation2D::Rotation2D(float x, float y) {
    theta_ = atan2(y, x);
}

Rotation2D Rotation2D::fromRadians(float radians) {
    return Rotation2D(radians);
}

Rotation2D Rotation2D::fromDegrees(float degrees) {
    return Rotation2D(degrees * (M_PI / 180.0));
}

float Rotation2D::getRadians() {
    return theta_;
}

float Rotation2D::getDegrees() {
    return theta_ * (180.0 / M_PI);
}

Rotation2D Rotation2D::operator+(const Rotation2D& other) {
    return rotateBy(other);
}

Rotation2D Rotation2D::operator-(const Rotation2D& other) {
    return rotateBy(other.inverse());
}

Rotation2D Rotation2D::operator*(const float& scalar) {
    return Rotation2D(theta_ * scalar);
}

Rotation2D Rotation2D::operator/(const float& scalar) {
    return Rotation2D(theta_ / scalar);
}

Rotation2D Rotation2D::inverse() const {
    return Rotation2D(-theta_);
}

Rotation2D Rotation2D::rotateBy(const Rotation2D& other) {
    return Rotation2D(cos(theta_) * cos(other.theta_) - sin(theta_) * sin(other.theta_),
                      cos(theta_) * sin(other.theta_) + sin(theta_) * cos(other.theta_));
}