/**
 * @file chassis_speed.hpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez & Dana E. Torres Estrada
 *  
 * @brief Header file for the ChassisSpeed class, which is the class for the chassis speed.
 */

#ifndef CHASSIS_SPEED_HPP
#define CHASSIS_SPEED_HPP

class ChassisSpeed {
    public:
        ChassisSpeed();
        ChassisSpeed(float vx, float vy, float omega);
        void setVx(float vx);
        void setVy(float vy);
        void setOmega(float omega);
        float getVx();
        float getVy();
        float getOmega();
    private:
        float vx_;
        float vy_;
        float omega_;
};

#endif