#ifndef DISTANCESENSOR_HPP
#define DISTANCESENSOR_HPP

#include <Arduino.h>
#include <vector>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"

using namespace Constants;

class DistanceSensor : public System{
    public:
        DistanceSensor();
        void begin() override;
        void update() override;
        void setState(int state) override;
        std::vector<float> getArrayDistance();
        float getDistance(int kSensor);
    private:
        float readSensor(uint8_t trigPin, uint8_t echoPin);
};

#endif