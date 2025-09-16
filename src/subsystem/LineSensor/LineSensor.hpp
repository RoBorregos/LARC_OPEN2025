#ifndef LINESENSOR_HPP
#define LINESENSOR_HPP

#include <Arduino.h>
#include <vector>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"

using namespace Constants;

class LineSensor : public System{
    public:
        LineSensor();
        void begin() override;
        void update() override;
        void setState(int state) override;
        std::vector<int> readSensors() const;

        bool isFrontLine() const;
        bool isBackLine() const;
        bool isLeftLine() const;
        bool isRightLine() const;

    private:
        int readSensor(int kSensor);
};

#endif