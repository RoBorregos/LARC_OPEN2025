/**
 * @file pose2d.cpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez & Dana E. Torres Estrada
 *  
 * @brief Implementation file for the Pose2D class, which is the class for the pose.
 */

#include "pose2d.hpp"

Pose2D::Pose2D(float x, float y, Rotation2D theta) 
    : x_(x), y_(y), theta_(theta)
{
}

float Pose2D::getX()
{
    return x_;
}

float Pose2D::getY()
{
    return y_;
}

Rotation2D Pose2D::getTheta()
{
    return theta_;
}

void Pose2D::setX(float x)
{
    x_ = x;
}

void Pose2D::setY(float y)
{
    y_ = y;
}

void Pose2D::setTheta(Rotation2D theta)
{
    theta_ = theta;
}