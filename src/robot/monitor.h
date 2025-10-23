#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "../robot/robot_instances.h"

class Monitor
{
public:
    Monitor(int rxPin, int txPin) : bluetooth(rxPin, txPin) {};

    void begin()
    {
        Serial.begin(9800);
        bluetooth.begin(9800);

        Serial.println("Beginning serial");
        bluetooth.println("Beginning bluetooth");
    }

    void print(const String &message)
    {
        // if (bluetooth)
        // {
        //     bluetooth.print(message);
        // }
        Serial.print(message);
    }

    void println(const String &message = "")
    {
        // if (bluetooth)
        // {
        //     bluetooth.println(message);
        // }
        Serial.println(message);
    }

private:
    SoftwareSerial bluetooth;
};