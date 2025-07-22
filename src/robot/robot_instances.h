#pragma once

#include "../subsystem/Drive/Drive.hpp"
#include "../subsystem/DistanceSensors/DistanceSensors.hpp"
#include "../subsystem/Communication/Communication.hpp"
#include "../subsystem/Elevator/Elevator.h"
#include "../subsystem/Gripper/Gripper.hpp"
#include "../subsystem/LowerSorter/LowerSorter.hpp"
#include "../subsystem/UpperSorter/UpperSorter.hpp"

extern Drive drive_;
extern DistanceSensors distance_sensor_;
extern Communication com_;
extern Elevator elevator_;
extern Gripper gripper_;
extern LowerSorter lower_sorter_;
extern UpperSorter upper_sorter_;
