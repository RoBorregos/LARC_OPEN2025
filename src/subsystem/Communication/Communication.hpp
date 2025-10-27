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
        string getCommand();
        void startMode();
        void sendData(const string &payload);
    private:
        string readCommunication();
        void sendMessage(const string &msg);
        enum class Mode { NONE=0, START=1, SEND_DATA=2 } mode_ = Mode::NONE;
};