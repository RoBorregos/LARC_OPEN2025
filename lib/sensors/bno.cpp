/*
 * @file bno.cpp
 * @date 2025-03-26
 * @author Juan Pablo Gutiérrez
 * 
 * @brief BNO sensor class
 */

#include "bno.hpp"

BNO::BNO() : bno(55, 0x28, &Wire), initialized(false) {
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

void BNO::getAngular(){
    update();
    
    float current_yaw = getYaw();
    float current_roll = getRoll(); 
    float current_pitch = getPitch();

    Serial.println("CURRENT ORIENTATION:");
    Serial.print("  Yaw (Z-axis):   "); Serial.print(current_yaw, 2); Serial.println("°");
    Serial.print("  Roll (Y-axis):  "); Serial.print(current_roll, 2); Serial.println("°");
    Serial.print("  Pitch (X-axis): "); Serial.print(current_pitch, 2); Serial.println("°");
}

void BNO::runCalibration() {
    Serial.println("====== BNO055 SENSOR TEST ======");
    
    if (!initialized) {
        Serial.println("ERROR: BNO055 not initialized! Call begin() first.");
        return;
    }
    
    Serial.println("1. SENSOR CONNECTION:");
    Serial.println("   BNO055 successfully initialized");
    Serial.print("   Temperature: ");
    Serial.print(bno.getTemp());
    Serial.println("°C");
    
    Serial.println("\n2. CALIBRATION STATUS:");
    uint8_t system_cal, gyro_cal, accel_cal, mag_cal;
    bno.getCalibration(&system_cal, &gyro_cal, &accel_cal, &mag_cal);
    Serial.print("   System: "); Serial.print(system_cal); Serial.println("/3");
    Serial.print("   Gyroscope: "); Serial.print(gyro_cal); Serial.println("/3");
    Serial.print("   Accelerometer: "); Serial.print(accel_cal); Serial.println("/3");
    Serial.print("   Magnetometer: "); Serial.print(mag_cal); Serial.println("/3");
    
    if (system_cal < 3) {
        Serial.println("   WARNING: Sensor not fully calibrated!");
        Serial.println("   Move the robot to calibrate (rotate in all axes)");
    } else {
        Serial.println("   ✓ Sensor fully calibrated!");
    }
    
    Serial.println("\n3. ORIENTATION TEST (10 readings):");
    Serial.println("   Reading | Yaw (Z) | Roll (Y) | Pitch (X) | Temp(°C)");
    Serial.println("   --------|---------|----------|-----------|----------");
    
    for (int i = 0; i < 10; i++) {
        update(); 
        
        float yaw = getYaw();
        float roll = getRoll();
        float pitch = getPitch();
        int8_t temp = bno.getTemp();
        
        Serial.print("      ");
        Serial.print(i + 1);
        Serial.print("     | ");
        Serial.print(yaw, 2);
        Serial.print("°   | ");
        Serial.print(roll, 2);
        Serial.print("°    | ");
        Serial.print(pitch, 2);
        Serial.print("°     | ");
        Serial.print(temp);
        Serial.println("°C");
        
        delay(500);
    }
    
    Serial.println("\n4. ANGULAR MOVEMENT TEST:");
    Serial.println("   Rotate the robot and observe changes:");
    
    float initial_yaw = getYaw();
    unsigned long test_start = millis();
    float max_yaw = initial_yaw;
    float min_yaw = initial_yaw;
    
    Serial.println("   Starting 15-second movement detection...");
    Serial.print("   Initial Yaw: "); Serial.print(initial_yaw, 2); Serial.println("°");
    
    while (millis() - test_start < 15000) {
        update();
        float current_yaw = getYaw();
        
        if (current_yaw > max_yaw) max_yaw = current_yaw;
        if (current_yaw < min_yaw) min_yaw = current_yaw;
        
        if ((millis() - test_start) % 2000 < 100) {
            Serial.print("   Current Yaw: "); 
            Serial.print(current_yaw, 2); 
            Serial.print("° | Range: ");
            Serial.print(min_yaw, 1);
            Serial.print("° to ");
            Serial.print(max_yaw, 1);
            Serial.println("°");
        }
        
        delay(100);
    }
    
    float total_rotation = max_yaw - min_yaw;
    Serial.print("   Total rotation detected: "); 
    Serial.print(total_rotation, 2); 
    Serial.println("°");
    
    if (total_rotation > 10) {
        Serial.println("   Movement detection working!");
    } else {
        Serial.println("   Low movement detected. Try rotating the robot more.");
    }
    
    Serial.println("\n5. STABILITY TEST (5 seconds):");
    Serial.println("   Keep robot still and observe stability:");
    
    update();
    float stable_yaw = getYaw();
    float drift_sum = 0;
    int drift_readings = 0;
    
    test_start = millis();
    while (millis() - test_start < 5000) {
        update();
        float current_yaw = getYaw();
        float drift = abs(current_yaw - stable_yaw);
        drift_sum += drift;
        drift_readings++;
        
        delay(100);
    }
    
    float average_drift = drift_sum / drift_readings;
    Serial.print("   Average drift: "); 
    Serial.print(average_drift, 3); 
    Serial.println("°");
    
    if (average_drift < 0.5) {
        Serial.println("   Excellent stability!");
    } else if (average_drift < 1.0) {
        Serial.println("   Good stability");
    } else {
        Serial.println("    High drift detected - check calibration");
    }
    
    Serial.println("\n6. WRAPPER FUNCTIONS TEST:");
    update();
    Serial.print("   Raw orientation.x (Yaw): "); Serial.print(event.orientation.x, 2); Serial.println("°");
    Serial.print("   getYaw() result: "); Serial.print(getYaw(), 2); Serial.println("°");
    Serial.print("   Wrapped angle test (-200°): "); Serial.print(wrapAngle(-200), 2); Serial.println("°");
    Serial.print("   Wrapped angle test (400°): "); Serial.print(wrapAngle(400), 2); Serial.println("°");
    
    Serial.println("\n====== BNO055 TEST COMPLETED ======");
    Serial.println("Test finished! Check results above.\n");
}