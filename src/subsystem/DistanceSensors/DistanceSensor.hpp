#pragma once

#include <Arduino.h>
#include <vector>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"

using namespace Constants;

class DistanceSensor : public System{
    public:
        DistanceSensor();
    private:
        std::vector<float> getArrayDistance();
        float getDistance(int kSensor);
};