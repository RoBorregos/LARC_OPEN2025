#include "DistanceSensors.hpp"

DistanceSensors::DistanceSensors() {
    pinMode(leftDistanceSensor, INPUT);
    pinMode(rightDistanceSensor, INPUT);
    pinMode(gripperDistanceSensor, INPUT);
}

float DistanceSensors::readSharpDistance(int sensorPin) {
    int analogValue = analogRead(sensorPin);

    float voltage = analogValue * (3.3 / 1023.0);
    float distanceCm = 27.728 * pow(voltage, -1.2045);

    if (distanceCm > 80) distanceCm = 80;
    if (distanceCm < 10) distanceCm = 10;

    return distanceCm;
}

float DistanceSensors::getLeftDistance() {
    return readSharpDistance(leftDistanceSensor);
}

float DistanceSensors::getRightDistance() {
    return readSharpDistance(rightDistanceSensor);
}

float DistanceSensors::getGripperDistance() {
    return readSharpDistance(gripperDistanceSensor);
}
