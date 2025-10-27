#include <Arduino.h>
#include "LineSensor.hpp"
#include "constants/pins.h"
#include <vector>
using namespace std;

LineSensor::LineSensor() : System()
{
}

void LineSensor::begin()
{
    pinMode(Pins::kLineSensorFL, INPUT);
    pinMode(Pins::kLineSensorFR, INPUT);
    pinMode(Pins::kLineSensorBL, INPUT);
    pinMode(Pins::kLineSensorBR, INPUT);
    pinMode(Pins::kLineSensorFC, INPUT);
}

void LineSensor::update() {}

void LineSensor::setState(int state) {}

std::vector<int> LineSensor::readSensors() const
{
    return {
        digitalRead(Pins::kLineSensorFL),
        digitalRead(Pins::kLineSensorFR),
        digitalRead(Pins::kLineSensorBL),
        digitalRead(Pins::kLineSensorBR),
        digitalRead(Pins::kLineSensorFC)};
}

void LineSensor::printSensors() const
{
    auto sensors = readSensors();
    Serial.print("FL: ");
    Serial.print(sensors[0]);
    Serial.print(" FR: ");
    Serial.print(sensors[1]);
    Serial.print(" BL: ");
    Serial.print(sensors[2]);
    Serial.print(" BR: ");
    Serial.println(sensors[3]);
    Serial.print(" FC: ");
    Serial.println(sensors[4]);
}

bool LineSensor::isFrontLine()
{
    auto v = readSensors();
    return v[0] || v[1] || v[4];
}

bool LineSensor::isBackLine()
{
    auto v = readSensors();
    return v[2] && v[3];
}

bool LineSensor::isLeftLine()
{
    auto v = readSensors();
    return v[0] || v[2];
}

bool LineSensor::isRightLine()
{
    auto v = readSensors();
    return v[1] || v[3];
}

bool LineSensor::isFrontLeftLine() const
{
    auto v = readSensors();
    return v[0];
}

bool LineSensor::isFrontRightLine() const
{
    auto v = readSensors();
    return v[1];
}

bool LineSensor::isBackLeftLine() const
{
    auto v = readSensors();
    return v[2];
}

bool LineSensor::isBackRightLine() const
{
    auto v = readSensors();
    return v[3];
}

bool LineSensor::isCenterLine() const
{
    auto v = readSensors();
    return v[4];
}

int LineSensor::readSensor(int kSensor)
{
    int value = digitalRead(kSensor);
    return value;
}