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
#include "subsystem/LineSensor/LineSensor.hpp"

StateManager state_manager;
DistanceSensors distance_sensor;
Drive drive;
LineSensor line_sensor;

const unsigned long UPDATE_INTERVAL = 50;

double state_start_time = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
}


void loop()
{
  
//  if (distance_sensor.getCurrentDistance() < 10 && line_sensor.leftDetected == 0 || line_sensor.rightDetected == 0) {
  if (distance_sensor.getCurrentDistance() < 10 && line_sensor.leftDetected == 0 || line_sensor.rightDetected == 0) {    
    drive.acceptInput(0, 200, 0);
  } else {
    drive.acceptInput(0, -200, 0);
  }
}

