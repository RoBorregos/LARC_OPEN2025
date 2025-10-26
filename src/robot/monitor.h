#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>

class Monitor
{
public:
    Monitor(int rxPin, int txPin) : bluetooth(rxPin, txPin) {};

    void begin()
    {
        Serial.begin(9600);
        bluetooth.begin(9800);

        Serial.println("Beginning serial");
        bluetooth.println("Beginning bluetooth");
    }

    void print(const String &message)
    {
        if (bluetoothEnabled)
        {
            bluetooth.print(message);
        }
        Serial.print(message);
    }

    void println(const String &message = "")
    {
        if (bluetoothEnabled)
        {
            bluetooth.println(message);
        }
        Serial.println(message);
    }

private:
    SoftwareSerial bluetooth;
    bool bluetoothEnabled = false;
};