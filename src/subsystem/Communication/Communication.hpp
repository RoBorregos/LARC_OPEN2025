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

        // Vision control
        void startDetection();
        void stopDetection();
        
        // Bean detection getters
        string getTopBean();
        string getBottomBean();
        bool isVisionActive();
        
        // Send data to MQTT via vision system
        void sendDataToMQTT(const string& data);
        
    private:

        enum class VisionState
        {
            GET_BEANS = 0,
            GET_BENEFICIADEROS = 1,
        };

        VisionState vision_state_ = VisionState::GET_BEANS;
        
        // Bean detection data
        string top_bean_;
        string bottom_bean_;
        bool vision_active_;
        
        // Private methods
        string readCommunication();
        void readVisionData();
};