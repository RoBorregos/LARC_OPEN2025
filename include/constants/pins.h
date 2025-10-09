#ifndef Pins_h
#define Pins_h

#include <Arduino.h>

namespace Pins {
    // READY
    constexpr uint8_t kPwmPin[4] = {
        10, // PWM1 UPPER LEFT MOTOR 
        5, // PWM2 UPPER RIGHT MOTOR
        29, // PWM3 BOTTOM LEFT MOTOR
        2, // PWM4 BOTTOM RIGHT MOTOR
    };
    
    // READY
    constexpr uint8_t kUpperMotors[4] = {
        11, // IN1.1 UPPER LEFT MOTOR 
        12, // IN1.2 UPPER LEFT MOTOR
        6, // IN2.1 UPPER RIGHT MOTOR
        9, // IN2.2 UPPER RIGHT MOTOR
    };

    // READY
    constexpr uint8_t kLowerMotors[4] = {
        31, // IN3.1 LOWER LEFT MOTOR
        32, // IN3.2 LOWER LEFT MOTOR
        3, // IN4.1 LOWER RIGHT MOTOR
        4  // IN4.2 LOWER RIGHT MOTOR
    }; 
    
    // READY
    constexpr uint8_t kEncoders[8]{
        40, // ENC1B
        39, // ENC1A
        15, // ENC2A
        14, // ENC2B
        41, // ENC3A
        13, // ENC3B
        17, // ENC4A
        16 // ENC4B
    };
    
    // NOT READY
    constexpr uint8_t kDistanceSensors[4][2] = {
        // trig, echo
        {35, 33},
        {36, 34},
    };
 

    // NOT READY
    constexpr uint8_t kSorterStepPin = 22;
    constexpr uint8_t kSorterDirPin = 7;

    // NOT READY
    constexpr uint8_t kSorterServo1Pin = 23;
    constexpr uint8_t kSorterServo2Pin = 24;
    constexpr uint8_t kSorterServo3Pin = 25;
    constexpr uint8_t kDropperServoPin = 26;

    // READY
    static constexpr uint8_t kLineSensorFL = 28;
    static constexpr uint8_t kLineSensorFR = 27;
    static constexpr uint8_t kLineSensorBL = 21;
    static constexpr uint8_t kLineSensorBR = 20;
}

#endif