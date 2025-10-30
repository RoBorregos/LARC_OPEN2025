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

static inline bool isValidDistance(float d)
{
    // Im gonna remove the d < DistanceSensorConstants::kMaxTargetDistance check to allow for farther readings to be considered valid
    // Add it again if it causes issues
    return std::isfinite(d) && d > 0.0f && !isnan(d) && d > 2.0f;
}

float DistanceSensor::readSensor(uint8_t trigPin, uint8_t echoPin) const
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 10000); //   timeout

    if (duration == 0)
    {
        return std::numeric_limits<float>::infinity();
    }

    float distance = duration * 0.0343 / 2.0;

    return distance;
}

std::pair<float, bool> DistanceSensor::getDistance(int kSensor)
{
    unsigned long &lastReadTime = (kSensor == 0) ? lastReadTimeLeft : lastReadTimeRight;
    auto &readings = (kSensor == 0) ? leftSensorReadings : rightSensorReadings;
    uint8_t trigPin = Pins::kDistanceSensors[kSensor][0];
    uint8_t echoPin = Pins::kDistanceSensors[kSensor][1];

    // Only read every 110 ms
    if (millis() - lastReadTime > 110)
    {
        float measurement = readSensor(trigPin, echoPin);
        bool isValid = isValidDistance(measurement);

        if (isValid)
        {
            if (kSensor == 0)
                insertReadingLeft(measurement);
            else
                insertReadingRight(measurement);
        }

        Serial.print(kSensor == 0 ? "Left Sensor Measurement: " : "Right Sensor Measurement: ");
        Serial.println(measurement);

        lastReadTime = millis();
    }

    // Always return the smoothed (averaged) value
    float sum = 0;
    for (float r : readings)
        sum += r;

    float average = readings.empty() ? 0 : sum / readings.size();
    bool valid = isValidDistance(average);

    return {average, valid};
}

std::pair<bool, bool> DistanceSensor::isObstacle()
{
    std::pair<float, bool> frontLeftDistance = getDistance(0);
    std::pair<float, bool> frontRightDistance = getDistance(1);
    bool obstacle = (frontLeftDistance.first < DistanceSensorConstants::kObstacleDistance) || (frontRightDistance.first < DistanceSensorConstants::kObstacleDistance);

    return {obstacle, frontLeftDistance.second || frontRightDistance.second};
}

void DistanceSensor::insertReadingLeft(float measurement)
{
    leftSensorReadings.push_back(measurement);

    if (leftSensorReadings.size() > 5)
    {
        leftSensorReadings.erase(leftSensorReadings.begin());
    }
}

void DistanceSensor::insertReadingRight(float measurement)
{
    rightSensorReadings.push_back(measurement);

    if (rightSensorReadings.size() > 5)
    {
        rightSensorReadings.erase(rightSensorReadings.begin());
    }
}

void DistanceSensor::clearReadings()
{
    leftSensorReadings.clear();
    rightSensorReadings.clear();
}