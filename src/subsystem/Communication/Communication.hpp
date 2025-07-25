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
        void update() override;
        void setState(int state) override;
        string getCommand();
    private:
        string readCommunication();
};