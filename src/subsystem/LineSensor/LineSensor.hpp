#ifndef LINESENSOR_HPP
#define LINESENSOR_HPP

#include <Arduino.h>
#include <vector>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"

using namespace Constants;

class LineSensor{
    public:
        LineSensor();
        std::vector<int> readSensors() const;
        int readSensor(int kSensor);
    private:
};

#endif