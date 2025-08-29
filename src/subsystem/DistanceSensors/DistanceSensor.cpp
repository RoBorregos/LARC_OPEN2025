#include "DistanceSensor.hpp"

DistanceSensor::DistanceSensor() {
    for (int i = 0; i < 4; i++) {
        pinMode(Pins::kDistanceSensors[i][0], OUTPUT); // TRIG
        pinMode(Pins::kDistanceSensors[i][1], INPUT);  // ECHO
    }
}

float DistanceSensor::getDistance(int index) {
    uint8_t trigPin = Pins::kDistanceSensors[index][0];
    uint8_t echoPin = Pins::kDistanceSensors[index][1];

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 30000);

    float distance = duration * 0.034 / 2.0;

    // Limitamos a un rango útil
    if (distance <= 0 || distance > 400) {
        distance = -1; // -1 significa sin lectura válida
    }

    return distance;
}

std::vector<float> DistanceSensor::getArrayDistance() {
    std::vector<float> distances;

    for (int i = 0; i < 4; i++) {
        float d = getDistance(i);
        distances.push_back(d);

        Serial.print("Sensor ");
        Serial.print(i + 1);
        Serial.print(": ");
        if (d == -1) {
            Serial.println("Fuera de rango");
        } else {
            Serial.print(d);
            Serial.println(" cm");
        }
    }

    delay(500);
    return distances;
}
