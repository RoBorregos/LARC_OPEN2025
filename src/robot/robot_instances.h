#pragma once

#include "../subsystem/Drive/Drive.hpp"
#include "../subsystem/DistanceSensors/DistanceSensor.hpp"
#include "../subsystem/Communication/Communication.hpp"
#include "../subsystem/Elevator/Elevator.h"
#include "../subsystem/LineSensor/LineSensor.hpp"

extern Drive drive_;
extern DistanceSensor distance_sensor_;
extern LineSensor line_sensor_;
extern Communication com_;
extern Elevator elevator_;