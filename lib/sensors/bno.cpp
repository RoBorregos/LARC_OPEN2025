/*
 * @file bno.cpp
 * @date 2025-03-26
 * @author Juan Pablo Gutiérrez
 * 
 * @brief BNO sensor class
 */

#include "bno.hpp"

BNO::BNO() : bno(55, 0x28, &Wire1), initialized(false) {
    begin();
}

bool BNO::begin() {
    Serial.println("Initializing BNO055");

    while (!bno.begin()) {
        Serial.println("Failed to initialize BNO055");
        delay(1000);
    }
    Serial.println("BNO055 initialized successfully");
    
    delay(1000); // Give the sensor time to initialize
    bno.setExtCrystalUse(true);
    initialized = true;
    return true;
}

/// @brief Update the BNO sensor
/// @note Call this function in the main loop
void BNO::update() {
    if (!initialized) return;
    bno.getEvent(&event);
}

float BNO::wrapAngle(float angle) const {
    // Normalize angle to [0, 360)
    angle = fmod(angle, 360.0f);
    if (angle < 0) {
        angle += 360.0f;
    }
    
    // Wrap to [-180, 180]
    if (angle > 180.0f) {
        angle -= 360.0f;
    } else if (angle < -180.0f) {
        angle += 360.0f;
    }
    
    return angle;
}

float BNO::getYaw() const {
    return -wrapAngle(event.orientation.x);
}

float BNO::getRoll() const {
    return event.orientation.y;
}

float BNO::getPitch() const {
    return event.orientation.z;
}