#include "DistanceSensor.hpp"
#include <Arduino.h>

DistanceSensor::DistanceSensor() : System(){
}

void DistanceSensor::begin() {
    pinMode(Pins::kLeftDistanceSensor, INPUT);
    pinMode(Pins::kRightDistanceSensor, INPUT);
}

void DistanceSensor::update() { }

void DistanceSensor::setState(int state) { }

std::vector<float> DistanceSensor::getArrayDistance() {
    float kLeftAnalog  = analogRead(Pins::kLeftDistanceSensor);
    float kRightAnalog = analogRead(Pins::kRightDistanceSensor);

    float leftVoltage  = kLeftAnalog * (5.0 / 1023.0);
    float rightVoltage = kRightAnalog * (5.0 / 1023.0);

    float kLeftDistance  = 27.728 * pow(leftVoltage, -1.2045);
    float kRightDistance = 27.728 * pow(rightVoltage, -1.2045);

    if (kLeftDistance > 80) kLeftDistance = 80;
    if (kLeftDistance < 10) kLeftDistance = 10;

    if (kRightDistance > 80) kRightDistance = 80;
    if (kRightDistance < 10) kRightDistance = 10;

    Serial.print("Left Distance: ");
    Serial.println(kLeftDistance);
    Serial.print("Right Distance: ");
    Serial.println(kRightDistance);

    delay(1000); 
    std::vector<float> distance = {kLeftDistance, kRightDistance};
    return distance;
}

float DistanceSensor::getDistance(int kSensor) {
    float rawValue = analogRead(kSensor);
    float voltage  = rawValue * (5.0 / 1023.0);
    float distance = 27.728 * pow(voltage, -1.2045);

    if (distance > 80) distance = 80;
    if (distance < 10) distance = 10;

    return distance;
}
