#include "DistanceSensors.hpp"

DistanceSensors::DistanceSensors() {
    pinMode(analogPin, INPUT);
}

float DistanceSensors::readSharpDistance() {
    int analogValue = analogRead(analogPin);

    float voltage = analogValue * (3.3 / 1023.0);
    float distanceCm = 27.728 * pow(voltage, -1.2045);

    if (distanceCm > 80) distanceCm = 80;
    if (distanceCm < 10) distanceCm = 10;

    return distanceCm;
}

float DistanceSensors::getCurrentDistance() {
    currentDistanceCm = readSharpDistance();
    return currentDistanceCm;
}