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
        string getCommand();

    private:
        int rxPin = Pins::kRx;
        int txPin = Pins::kTx;

        string readCommunication();
};