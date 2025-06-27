/*
 * @file Camera.h
 * @date 01/05/2025
 * @author Dana E. Torres Estrada
 *
 * @brief Header file for receiving Camera data from serial
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include "../systems/system.hpp"
#include "constants/constants.h"
using namespace Constants;

class Camera : public System {
private:
    // Lower camera state
    enum class CameraState {
        AVOID_OBSTACLES = 0,
        FETCH_TREES = 1,
        FETCH_STOREHOUSE_MADURO = 2,
        FETCH_STOREHOUSE_SOBREMADURO = 3,
    };

    // Lower camera variables
    float current_offset_x_ = 0.0;
    float current_offset_y_ = 0.0;
    float current_distance_ = 0.0;
    bool object_present_ = false;
    bool object_too_close_ = false;
    bool bean_storage_type = false; // False = MADURO, True = SOBREMADURO
    CameraState camera_state_ = CameraState::AVOID_OBSTACLES;

    // Upper camera variables
    float upper_offset_x_ = 0.0;
    float upper_offset_y_ = 0.0;
    BeanConstants::BeanType current_bean_type_ = BeanConstants::NONE;

public:
    Camera(); // Constructor
    
    // Lower camera methods
    float getOffset_X();
    float getOffset_Y();
    float getObjectDistance();
    bool objectPresent();
    bool isObjectTooClose();
    void setState(int state) override;

    // Upper camera methods
    BeanConstants::BeanType getBeanType();
    float getBeanOffset_X();
    float getBeanOffset_Y();

    // Method to receive Serial data from the raspberry
    void update() override;
};

#endif // CAMERA_H 