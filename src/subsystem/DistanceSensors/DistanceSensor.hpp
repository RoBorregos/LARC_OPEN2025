#ifndef DISTANCESENSOR_HPP
#define DISTANCESENSOR_HPP

#include <Arduino.h>
#include <vector>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"
#include "queue"

using namespace Constants;

class DistanceSensor : public System
{
public:
    DistanceSensor();
    void begin() override;
    void update() override;
    void setState(int state) override;

    // The first element of the pair is the distance in cm, the second is whether the reading is valid
    std::pair<bool, bool> isObstacle();
    std::pair<bool, bool> isTree();
    std::pair<float, bool> getDistance(int kSensor);
    void clearReadings();

private:
    void insertReadingLeft(float measurement);
    void insertReadingRight(float measurement);


    float readSensor(uint8_t trigPin, uint8_t echoPin) const;
    std::vector<float> leftSensorReadings;
    std::vector<float> rightSensorReadings;
};

#endif