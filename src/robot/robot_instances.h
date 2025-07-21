#pragma once

#include "../subsystem/Drive/Drive.hpp"
#include "../subsystem/DistanceSensors/DistanceSensors.hpp"
#include "../subsystem/LineSensor/LineSensor.hpp"
#include "../subsystem/Communication/Communication.hpp"
#include "../subsystem/Elevator/Elevator.hpp"
#include "../subsystem/Gripper/Gripper.hpp"
#include "../subsystem/LowerSorter/LowerSorter.hpp"
#include "../subsystem/UpperSorter/UpperSorter.hpp"

extern Drive drive_;
extern LineSensor line_sensor_;
extern DistanceSensors distance_sensor_;
extern Communication com_;
extern Elevator elevator_;
extern Gripper gripper_;
extern LowerSorter lower_sorter_;
extern UpperSorter upper_sorter_;
// extern PIDController centerPID_;
// extern LineSensor line_sensor_;
// extern Camera camera_;