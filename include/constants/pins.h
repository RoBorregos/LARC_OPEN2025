#ifndef Pins_h
#define Pins_h

#include <Arduino.h>

namespace Pins {
    // READY
    constexpr uint8_t kPwmPin[5] = {
        2, // PWM1 UPPER LEFT MOTOR 
        3, // PWM2 UPPER RIGHT MOTOR
        4, // PWM3 BOTTOM LEFT MOTOR
        5, // PWM4 BOTTOM RIGHT MOTOR
        0  // PWM5 FOR THE ELEVATOR (YET TO DEFINE)
    };
    
    // READY
    constexpr uint8_t kUpperMotors[4] = {
        22, // IN1.1 UPPER LEFT MOTOR 
        23, // IN1.2 UPPER LEFT MOTOR
        26, // IN2.1 UPPER RIGHT MOTOR
        27, // IN2.2 UPPER RIGHT MOTOR
    };

    //READY
    constexpr uint8_t kLowerMotors[4] = {
        28, // IN3.1 LOWER LEFT MOTOR
        29, // IN3.2 LOWER LEFT MOTOR
        30, // IN4.1 LOWER RIGHT MOTOR
        31  // IN4.2 LOWER RIGHT MOTOR
    };  

    // NOT YET
    constexpr uint8_t kElevatorINA[2] = {
        10,
        11
    };
    
    // YET TO IMPLEMENT
    constexpr uint8_t kEncoders[10]{
        32, // ENC1A
        36, // ENC1B
        33, // ENC2A
        37, // ENC2B
        34, // ENC3A
        38, // ENC3B
        35, // ENC4A
        39, // ENC4B
        0, // ENC ELEVATORA
        0  // ENC ELEVATORB
    };

    // UNUSED
    constexpr uint8_t kGripperServoPin = 0;
    constexpr uint8_t kUpperSorterServoPin = 0;
    constexpr uint8_t kLowerSorterServoPin = 0;
    constexpr uint8_t kLimitPin = 0;

    // NOT YET
    static constexpr uint8_t kLineSensorFL = 40;
    static constexpr uint8_t kLineSensorFR = 41;
    static constexpr uint8_t kLineSensorBL = 0;
    static constexpr uint8_t kLineSensorBR = 0;
    
    // READY
    constexpr uint8_t kLeftDistanceSensor = A1;
    constexpr uint8_t kRightDistanceSensor = A0;
}

#endif