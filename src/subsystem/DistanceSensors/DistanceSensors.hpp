#pragma once

#include <Arduino.h>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"

using namespace Constants;

class DistanceSensors : public System {
public:
    DistanceSensors();
    float getCurrentDistance(); 
private:
    int analogPin = Pins::kDistanceSensor; 
    float currentDistanceCm = 0.0;

    float readSharpDistance();
};
