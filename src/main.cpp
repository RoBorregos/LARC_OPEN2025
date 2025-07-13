/**
 * LARC 2025
 *
 * @Roborregos
 */

#include <Arduino.h>
#include <Wire.h>
#include "pose2d.hpp"
#include "robot/statemanager.hpp"
#include "subsystem/Drive/Drive.hpp"
#include "subsystem/DistanceSensors/DistanceSensors.hpp"

StateManager state_manager;
DistanceSensors distance_sensor;
Drive drive;

const unsigned long UPDATE_INTERVAL = 50;

double state_start_time = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
}


void loop()
{
  if (distance_sensor.getCurrentDistance() < 10) {
    drive.acceptInput(0, 200, 0);
  } else {
    drive.acceptInput(200, 0, 0);
  }
}

