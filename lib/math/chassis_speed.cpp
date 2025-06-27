/**
 * @file chassis_speed.cpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez & Dana E. Torres Estrada
 *  
 * @brief Implementation file for the ChassisSpeed class, which is the class for the chassis speed.
 */

#include "chassis_speed.hpp"

ChassisSpeed::ChassisSpeed() : vx_(0), vy_(0), omega_(0) {
}

ChassisSpeed::ChassisSpeed(float vx, float vy, float omega) : vx_(vx), vy_(vy), omega_(omega) {
}

void ChassisSpeed::setVx(float vx) {
    vx_ = vx;
}

void ChassisSpeed::setVy(float vy) {
    vy_ = vy;
}

void ChassisSpeed::setOmega(float omega) {
    omega_ = omega;
}

float ChassisSpeed::getVx() {
    return vx_;
}

float ChassisSpeed::getVy() {
    return vy_;
}

float ChassisSpeed::getOmega() {
    return omega_;
}