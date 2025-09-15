#include "DistanceSensor.hpp"
#include "../include/constants/pins.h"

DistanceSensor::DistanceSensor() : System()
{
}

void DistanceSensor::begin()
{
    pinMode(Pins::kDistanceSensors[0][0], OUTPUT);
    pinMode(Pins::kDistanceSensors[0][1], INPUT);
    pinMode(Pins::kDistanceSensors[1][0], OUTPUT);
    pinMode(Pins::kDistanceSensors[1][1], INPUT);
}

void DistanceSensor::update() {}
void DistanceSensor::setState(int state) {}

float DistanceSensor::readSensor(uint8_t trigPin, uint8_t echoPin)
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 30000);

    float distance = duration * 0.0343 / 2.0;

    return distance;
}

std::vector<float> DistanceSensor::getArrayDistance()
{
    float kLeftDistance = readSensor(Pins::kDistanceSensors[0][0], Pins::kDistanceSensors[0][1]);
    delay(50);
    float kRightDistance = readSensor(Pins::kDistanceSensors[1][0], Pins::kDistanceSensors[1][1]);

    /* For Debugging
    Serial.print("Left Distance: ");
    if (kLeftDistance == -1.0f) {
        Serial.print("Fuera de rango");
    } else {
        Serial.print(kLeftDistance);
        Serial.print(" cm");
    }

    Serial.print(" | Right Distance: ");
    if (kRightDistance == -1.0f) {
        Serial.println("Fuera de rango");
    } else {
        Serial.print(kRightDistance);
        Serial.println(" cm");
    }
    */

    std::vector<float> distances = {kLeftDistance, kRightDistance};
    return distances;
}

float DistanceSensor::getDistance(int kSensor)
{
    if (kSensor == 0)
    {
        return readSensor(Pins::kDistanceSensors[0][0], Pins::kDistanceSensors[0][1]);
    }
    else if (kSensor == 1)
    {
        return readSensor(Pins::kDistanceSensors[1][0], Pins::kDistanceSensors[1][1]);
    }
    return -1.0f;
}