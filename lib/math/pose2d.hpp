/**
 * @file pose2d.hpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez & Dana E. Torres Estrada
 *  
 * @brief Header file for the Pose2D class, which is the class for the pose.
 */

#ifndef POSE2D_HPP
#define POSE2D_HPP

#include "rotation2d.hpp"

class Pose2D {
    public:
        Pose2D(float x, float y, Rotation2D theta);
        float getX();
        float getY();
        Rotation2D getTheta();
        void setX(float x);
        void setY(float y);
        void setTheta(Rotation2D theta);
    private:
        float x_;
        float y_;
        Rotation2D theta_;
};

#endif