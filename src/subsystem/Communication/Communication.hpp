#pragma once

#include <Arduino.h>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"
#include <string>

using namespace Constants;
using namespace std;

struct ObstacleData {
    bool detected;
    float distance;
    float angle;
    unsigned long timestamp;
};

class Communication : public System {
    public:
        Communication();
        void begin() override;
        void update() override;
        void setState(int state) override;
        
        // Command processing
        string getCommand();
        
        // Vision data processing
        ObstacleData getObstacleData();
        bool hasNewObstacleData();
        
        // Send commands to vision system
        void requestObstacleDetection();
        void stopDetection();
        
    private:
        string readCommunication();
        void parseObstacleMessage(const string& message);
        void parseVisionMessage(const string& message);
        
        // Vision data storage
        ObstacleData latest_obstacle_data;
        bool new_obstacle_data;
        bool new_bean_data;
};