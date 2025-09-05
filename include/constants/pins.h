#ifndef Pins_h
#define Pins_h

#include <Arduino.h>

namespace Pins {
    // READY
    constexpr uint8_t kPwmPin[5] = {
        26, // PWM1 UPPER LEFT MOTOR 
        5, // PWM2 UPPER RIGHT MOTOR
        10, // PWM3 BOTTOM LEFT MOTOR
        2, // PWM4 BOTTOM RIGHT MOTOR
        0  // PWM5 FOR THE ELEVATOR (YET TO DEFINE)
    };
    
    // READY
    constexpr uint8_t kUpperMotors[4] = {
        32, // IN1.1 UPPER LEFT MOTOR 
        31, // IN1.2 UPPER LEFT MOTOR
        9, // IN2.1 UPPER RIGHT MOTOR
        6, // IN2.2 UPPER RIGHT MOTOR
    };

    //READY
    constexpr uint8_t kLowerMotors[4] = {
        12, // IN3.1 LOWER LEFT MOTOR
        11, // IN3.2 LOWER LEFT MOTOR
        4, // IN4.1 LOWER RIGHT MOTOR
        3  // IN4.2 LOWER RIGHT MOTOR
    };  

    // NOT YET
    constexpr uint8_t kElevatorINA[2] = {
        10,
        11
    };
    
    
    constexpr uint8_t kDistanceSensors[4] = {
        27, // TRIG_LEFT
        26, // ECHO_LEFT
        38, // TRIG_RIGHT
        39 // ECHO_RIGHT
    };

    // YET TO IMPLEMENT
    constexpr uint8_t kEncoders[10]{
        41, // ENC1A
        40, // ENC1B
        23, // ENC2A
        22, // ENC2B
        14, // ENC3A
        15, // ENC3B
        16, // ENC4A
        17, // ENC4B
        0, // ENC ELEVATORA
        0  // ENC ELEVATORB
    };

    // UNUSED
    constexpr uint8_t kGripperServoPin = 0;
    constexpr uint8_t kUpperSorterServoPin = 0;
    constexpr uint8_t kLowerSorterServoPin = 0;
    constexpr uint8_t kLimitPin = 0;

    // NOT YET
    static constexpr uint8_t kLineSensorFL = 36;
    static constexpr uint8_t kLineSensorFR = 37;
    static constexpr uint8_t kLineSensorBL = 34;
    static constexpr uint8_t kLineSensorBR = 13;
}

#endif