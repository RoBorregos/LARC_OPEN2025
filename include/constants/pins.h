#ifndef Pins_h
#define Pins_h

#include <Arduino.h>

namespace Pins {
    constexpr uint8_t kPwmPin[5] = {
        29, // PWM1 UPPER LEFT MOTOR 
        2, // PWM2 UPPER RIGHT MOTOR
        10, // PWM3 BOTTOM LEFT MOTOR
        5, // PWM4 BOTTOM RIGHT MOTOR
        0  // PWM5 FOR THE ELEVATOR (YET TO DEFINE)
    };
    
    constexpr uint8_t kUpperMotors[4] = {
        31, // IN1.1 UPPER LEFT MOTOR 
        32, // IN1.2 UPPER LEFT MOTOR
        4, // IN2.1 UPPER RIGHT MOTOR
        3, // IN2.2 UPPER RIGHT MOTOR
    };

    constexpr uint8_t kLowerMotors[4] = {
        11, // IN3.1 LOWER LEFT MOTOR
        12, // IN3.2 LOWER LEFT MOTOR
        9, // IN4.1 LOWER RIGHT MOTOR
        6  // IN4.2 LOWER RIGHT MOTOR
    };  
    
    constexpr uint8_t kDistanceSensors[4][2] = {
        {39, 27},
        {40, 26},
        {1, 2},
        {3, 4}
    };

    constexpr uint8_t kEncoders[8]{
        14, // ENC1A
        15, // ENC1B
        20, // ENC2A
        21, // ENC2B
        16, // ENC3A
        17, // ENC3B
        22, // ENC4A
        23 // ENC4B
    };
    
    // LineSensors 
    constexpr uint8_t kLineSensorFL = 34;
    constexpr uint8_t kLineSensorFR = 36;
    constexpr uint8_t kLineSensorBL = 33;
    constexpr uint8_t kLineSensorBR = 37;

    // Servos for the Intake 
    constexpr uint8_t kUpperServo = 0;
    constexpr uint8_t kMiddleServo = 0;
    constexpr uint8_t kLowerServo = 0;
    
    // Elevator Pins
    constexpr uint8_t kDirElevator = 0;
    constexpr uint8_t kStepElevator = 0;
    
}

#endif