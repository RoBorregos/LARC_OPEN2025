#ifndef Pins_h
#define Pins_h

#include <Arduino.h>

namespace Pins {
    constexpr uint8_t kPwmPin[5] = {
        12, // PWM1 UPPER LEFT MOTOR 
        24, // PWM2 UPPER RIGHT MOTOR
        25, // PWM3 BOTTOM LEFT MOTOR
        33, // PWM4 BOTTOM RIGHT MOTOR
        19  // PWM5 FOR THE ELEVATOR
    };
    
    constexpr uint8_t kUpperMotors[4] = {
        2, // IN1.1 UPPER LEFT MOTOR 
        3, // IN1.2 UPPER LEFT MOTOR
        4, // IN2.1 UPPER RIGHT MOTOR
        5, // IN2.2 UPPER RIGHT MOTOR
    };

    constexpr uint8_t kLowerMotors[4] = {
        6, // IN3.1 LOWER LEFT MOTOR
        7, // IN3.2 LOWER LEFT MOTOR
        8, // IN4.1 LOWER RIGHT MOTOR
        9  // IN4.2 LOWER RIGHT MOTOR
    };  

    constexpr uint8_t kElevatorINA[2] = {
        10,
        11
    };
    
    constexpr uint8_t kEncoders[10]{
        27, // ENC1A
        28, // ENC1B
        29, // ENC2A
        30, // ENC2B
        31, // ENC3A
        32, // ENC3B
        23, // ENC4A
        22, // ENC4B
        21, // ENC ELEVATORA
        20  // ENC ELEVATORB
    };

    constexpr uint8_t kGripperServoPin = 15;
    constexpr uint8_t kUpperSorterServoPin = 14;
    constexpr uint8_t kLowerSorterServoPin = 13;

    constexpr uint8_t kLimitPin = 39;

    constexpr uint8_t kLineSensorLeftPin = 38;
    constexpr uint8_t kLineSensorRightPin = 37;
    
    constexpr uint8_t kLeftDistanceSensor = 41;
    constexpr uint8_t kRightDistanceSensor = 40;
    constexpr uint8_t kGripperDistanceSensor = 36;
}

#endif