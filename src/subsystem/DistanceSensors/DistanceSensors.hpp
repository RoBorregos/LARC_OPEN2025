#pragma once

#include <Arduino.h>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"

using namespace Constants;

class DistanceSensors : public System {
public:
    DistanceSensors();
    void update() override;
    void setState(int state) override;
    float getCurrentDistance(); 
    float getLeftDistance();
    float getRightDistance();
    float getGripperDistance();
private:
    int rightDistanceSensor = Pins::kRightDistanceSensor;
    int leftDistanceSensor = Pins::kLeftDistanceSensor;
    int gripperDistanceSensor = Pins::kGripperDistanceSensor;
    
    float currentDistanceCm = 0.0;

    float readSharpDistance(int sensorPin);
};