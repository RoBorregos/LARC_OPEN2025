#pragma once

#include <Arduino.h>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"
#include <string>

using namespace Constants;
using namespace std;

class Communication : public System {
    public:
        Communication();
        void begin() override;
        void update() override;
        void setState(int state) override;
        
        // Command processing
        string getCommand();

        void startDetection();
        void stopDetection();
        
    private:

        enum class VisionState
        {
            GET_BEANS = 0,
            GET_BENEFICIADEROS = 1,
        };

        VisionState vision_state_ = VisionState::GET_BEANS;
        string readCommunication();
};