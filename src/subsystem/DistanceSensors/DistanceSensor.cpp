#include "DistanceSensor.hpp"
#include <Arduino.h>

DistanceSensor::DistanceSensor(){
    pinMode(Pins::kLeftDistanceSensor, INPUT);
    pinMode(Pins::kRightDistanceSensor, INPUT);
}

std::vector<float> DistanceSensor::getArrayDistance(){
    float kRightAnalog = analogRead(Pins::kLeftDistanceSensor);
    float kLeftAnalog = analogRead(Pins::kRightDistanceSensor);

    float rightVoltage = kRightAnalog * (5 / 1023.0);
    float kRightDistance = 27.728 * pow(rightVoltage, -1.2045);
    if (kRightDistance > 80) kRightDistance = 80;
    if (kRightDistance < 10) kRightDistance = 10;

    float leftVoltage = kLeftAnalog * (5 / 1023.0);
    float kLeftDistance = 27.728 * pow(leftVoltage, -1.2045);
    if (kLeftDistance > 80) kLeftDistance = 80;
    if (kLeftDistance < 10) kLeftDistance = 10;

    std::vector<float> distance = {kRightDistance, kLeftDistance};

    return distance;
}

float DistanceSensor::getDistance(int kSensor){
    float Distance = analogRead(kSensor);

    return Distance;
}