#ifndef DISTANCESENSOR_HPP
#define DISTANCESENSOR_HPP

#include <Arduino.h>
#include <vector>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"

using namespace Constants;

class DistanceSensor{
    public:
        DistanceSensor();
        std::vector<float> getArrayDistance();
        float getDistance(int kSensor);
    private:
};

#endif