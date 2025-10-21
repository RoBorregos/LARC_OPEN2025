#ifndef LINESENSOR_HPP
#define LINESENSOR_HPP

#include <Arduino.h>
#include <vector>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"
#include "../../lib/sensors/LineSensorChecker.h"

using namespace Constants;

class LineSensor : public System
{
public:
    LineSensor();
    void begin() override;
    void update() override;
    void setState(int state) override;
    std::vector<int> readSensors() const;
    void printSensors() const;

    bool isFrontLine();
    bool isBackLine();
    bool isLeftLine();
    bool isRightLine();

    bool isFrontLeftLine() const;
    bool isFrontRightLine() const;
    bool isBackLeftLine() const;
    bool isBackRightLine() const;

private:
    int readSensor(int kSensor);

    LineSensorChecker leftLineChecker{
        [this]()
        { return this->isFrontLeftLine(); },
        [this]()
        { return this->isBackLeftLine(); },
        1};
};

#endif