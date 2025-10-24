#include <Arduino.h>
#include <Wire.h>
#include "../robot/robot_instances.h"

void setup()
{
  Serial.begin(9600);
  distance_sensor_.begin();
}

void loop()
{
  distance_sensor_.update();
  auto [distanceLeft, validLeft] = distance_sensor_.getDistance(0);
  auto [distanceRight, validRight] = distance_sensor_.getDistance(1);
  Serial.print("Left distance: ");

  Serial.print(distanceLeft);
  Serial.print(" cm  Valid: ");
  Serial.print(validLeft ? "Yes " : "No ");
  Serial.print(" cm  Right distance: ");
  Serial.print(distanceRight);
  Serial.print(" cm  Valid: ");
  Serial.print(validRight ? "Yes " : "No ");

  auto [isObstacle, obstacleValid] = distance_sensor_.isObstacle();
  Serial.print(isObstacle ? "Obstacle yes" : "Obstacle no");
  Serial.println();
  delay(100);
}