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

float DistanceSensor::readSensor(uint8_t trigPin, uint8_t echoPin) const
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

std::vector<float> DistanceSensor::getArrayDistance() const
{
    float kLeftDistance = readSensor(Pins::kDistanceSensors[0][0], Pins::kDistanceSensors[0][1]);
    delay(50);
    float kRightDistance = readSensor(Pins::kDistanceSensors[1][0], Pins::kDistanceSensors[1][1]);
    
    if(kLeftDistance > 250){
        kLeftDistance = 250;
    }
    if(kRightDistance > 250){
        kRightDistance = 250;
    }

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

bool DistanceSensor::isObstacle() const {
    auto distanceValues = getArrayDistance();
    int frontLeftDistance = distanceValues[0];
    int frontRightDistance = distanceValues[1];
    // bool obstacle = (((frontLeftDistance > 0 && frontLeftDistance < DistanceSensorConstants::kObstacleDistance)) || ((frontRightDistance > 0 && frontRightDistance < DistanceSensorConstants::kObstacleDistance)));

    bool obstacle = (frontLeftDistance < DistanceSensorConstants::kObstacleDistance);

    return obstacle;
}

bool DistanceSensor::obstacleInThePath() const {
    auto distanceValues = getArrayDistance();
    int frontLeftDistance = distanceValues[0];
    int frontRightDistance = distanceValues[1];

    // bool obstacle = (frontLeftDistance < DistanceSensorConstants::kMaxObstacleDistance) && (frontRightDistance < DistanceSensorConstants::kMaxObstacleDistance);
    bool obstacle = (frontLeftDistance < DistanceSensorConstants::kMaxObstacleDistance);

    return obstacle;
}

bool DistanceSensor::isTree() const{
    auto distanceValues = getArrayDistance();
    int frontLeftDistance = distanceValues[0];
    int frontRightDistance = distanceValues[1];

    bool tree = (frontLeftDistance < DistanceSensorConstants::kTreeDistance) || (frontRightDistance < DistanceSensorConstants::kTreeDistance);

    return tree;
}