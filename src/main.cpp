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
#include "subsystem/UpperSorter/UpperSorter.hpp"
#include "subsystem/LowerSorter/LowerSorter.hpp"
#include "subsystem/Gripper/Gripper.hpp"

StateManager state_manager;
DistanceSensors distance_sensor;
Drive drive;
LineSensor line_sensor;
LowerSorter lower_sorter;
UpperSorter upper_sorter;
Gripper gripper;

const unsigned long UPDATE_INTERVAL = 50;

double state_start_time = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
}


void loop()
{
  distance_sensor.getCurrentDistance();
  line_sensor.leftDetected();
  line_sensor.rightDetected();
  upper_sorter.setState(0);
  lower_sorter.setState(1);
  gripper.setState(1);
}

