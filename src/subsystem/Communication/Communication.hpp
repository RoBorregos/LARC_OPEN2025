#pragma once

#include <Arduino.h>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"
#include <string>
#include <vector>

using namespace Constants;
using namespace std;

class Communication : public System {
    public:
        Communication();
        void begin() override;
        void update() override;
        void setState(int state) override;
        string getCommand();
        void sendData(const string &payload);
        vector<int> getMatrix(const string &msg);
    private:
        string readCommunication();
        void sendMessage(const string &msg);
};