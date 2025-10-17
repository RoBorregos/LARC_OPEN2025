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
    return std::isfinite(d) && d > 0.0f && !isnan(d);
}

float DistanceSensor::readSensor(uint8_t trigPin, uint8_t echoPin) const
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);

    if (duration == 0)
    {
        return std::numeric_limits<float>::infinity();
    }

    float distance = duration * 0.0343 / 2.0;

    return distance;
}

float DistanceSensor::getDistance(int kSensor)
{
    if (kSensor == 0)
    {
        float measurement = readSensor(Pins::kDistanceSensors[0][0], Pins::kDistanceSensors[0][1]);
        
        // This will only insert valid measurements into the readings vector, if there is not a valid measurement it will keep the previous readings
        if (isValidDistance(measurement))
        {
            insertReadingLeft(measurement);
        }

        float sum = 0;
        for (float reading : leftSensorReadings)
        {
            sum += reading;
        }
        float average = leftSensorReadings.size() > 0 ? sum / leftSensorReadings.size() : 0;

        return average;
    }
    else if (kSensor == 1)
    {
        float measurement = readSensor(Pins::kDistanceSensors[1][0], Pins::kDistanceSensors[1][1]);

        // This will only insert valid measurements into the readings vector, if there is not a valid measurement it will keep the previous readings
        if (isValidDistance(measurement))
        {
            insertReadingRight(measurement);
        }

        float sum = 0;
        for (float reading : rightSensorReadings)
        {
            sum += reading;
        }
        float average = rightSensorReadings.size() > 0 ? sum / rightSensorReadings.size() : 0;

        return average;
    }

    return -1.0f;
}

bool DistanceSensor::isObstacle()
{
    float frontLeftDistance = getDistance(0);
    float frontRightDistance = getDistance(1);
    bool obstacle = (frontLeftDistance < DistanceSensorConstants::kObstacleDistance) || (frontRightDistance < DistanceSensorConstants::kObstacleDistance);

    return obstacle;
}

bool DistanceSensor::isTree()
{
    float frontLeftDistance = getDistance(0);
    float frontRightDistance = getDistance(1);

    bool tree = (frontLeftDistance < DistanceSensorConstants::kTreeDistance) || (frontRightDistance < DistanceSensorConstants::kTreeDistance);

    return tree;
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