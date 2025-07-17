#ifndef Pins_h
#define Pins_h

#include <Arduino.h>

namespace Pins {
    constexpr uint8_t kPwmPin[4] = {
        24,
        25,
        28,
        29
    };
    
    constexpr uint8_t kUpperMotors[4] = {
        0,
        1,
        2,
        3,
    };

    constexpr uint8_t kLowerMotors[4] = {
        4,
        5,
        6,
        7
    };  
    
    constexpr uint8_t kEncoders[4]{
        32,
        33,
        34,
        35
    };

    constexpr uint8_t kGripperServoPin = 15;
    constexpr uint8_t kUpperSorterServoPin = 14;
    constexpr uint8_t kLowerSorterServoPin = 13;

    constexpr uint8_t kLineSensorLeft = 23;
    constexpr uint8_t kLineSensorRight = 20;
    constexpr uint8_t kLimitPin = 12;

    constexpr uint8_t kDistanceAnalog = 41;

    constexpr uint8_t kRx = 21;
    constexpr uint8_t kTx = 20;
}

#endif