#ifndef Pins_h
#define Pins_h

#include <Arduino.h>

namespace Pins {
    constexpr uint8_t kPwmPin[4] = {
        24, // PWM2 UPPER LEFT MOTOR (PURPLE CABLE) 
        25,  // PWM4 UPPER RIGHT MOTOR (GRAY CABLE)
        28, // PWM3 BOTTOM LEFT MOTOR (GRAY CABLE)
        29  //  PWM1 BOTTOM RIGHT MOTOR (PURPLE CABLE)
    };
    
    constexpr uint8_t kUpperMotors[4] = {
        0, // IN1.1 UPPER LEFT MOTOR 
        1, // IN1.2 UPPER LEFT MOTOR
        2, // IN2.1 UPPER RIGHT MOTOR
        3, // IN2.2 UPPER RIGHT MOTOR
    };

    constexpr uint8_t kLowerMotors[4] = {
        4, // IN3.1 LOWER LEFT MOTOR
        5, // IN3.2 LOWER LEFT MOTOR
        6, // IN4.1 LOWER RIGHT MOTOR
        7  // IN4.2 LOWER RIGHT MOTOR
    };  
    
    constexpr uint8_t kEncoders[4]{
        32, // ENC 1 UPPER LEFT MOTOR
        33, // ENC 2 UPPER RIGHT MOTOR
        34, //ENC 3 LOWER LEFT MOTOR
        35 //ENC 4 LOWER RIGHT MOTOR
    };

    constexpr uint8_t kGripperServoPin = 15;
    constexpr uint8_t kUpperSorterServoPin = 14;
    constexpr uint8_t kLowerSorterServoPin = 13;

    constexpr uint8_t kStepperPin = 10;
    constexpr uint8_t kDirPin = 11;
    constexpr uint8_t kLimitPin = 12;

    constexpr uint8_t kLineSensorLeftPin = 2;
    constexpr uint8_t kLineSensorRightPin = 3;
}

#endif