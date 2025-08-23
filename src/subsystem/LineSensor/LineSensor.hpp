#pragma once

#include <Arduino.h>
#include <vector>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"

using namespace Constants;

class LineSensor : public System{
    public:
        LineSensor();
        vector<int> readSensors() const;
    private:
        int readSensor(int kSensor);
};