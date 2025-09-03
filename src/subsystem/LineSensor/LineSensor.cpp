#include <Arduino.h>
#include "LineSensor.hpp"
#include "constants/pins.h"
#include <vector>
using namespace std;

LineSensor::LineSensor() : System(){
}

void LineSensor::begin() {
    pinMode(Pins::kLineSensorFL, INPUT);
    pinMode(Pins::kLineSensorFR, INPUT);
    pinMode(Pins::kLineSensorBL, INPUT);
    pinMode(Pins::kLineSensorBR, INPUT);
}

void LineSensor::update() { }

void LineSensor::setState(int state) { }

std::vector<int> LineSensor::readSensors() const {
    return {
        digitalRead(Pins::kLineSensorFL),
        digitalRead(Pins::kLineSensorFR),
        digitalRead(Pins::kLineSensorBL),
        digitalRead(Pins::kLineSensorBR)
    };
}

int LineSensor::readSensor(int kSensor){
    int value = digitalRead(kSensor);
    return value; 
}