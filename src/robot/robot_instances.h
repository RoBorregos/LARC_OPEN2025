#pragma once

#include "../subsystem/Drive/Drive.h"
#include "../subsystem/DistanceSensors/DistanceSensor.hpp"
#include "../subsystem/LineSensor/LineSensor.hpp"
#include "../subsystem/Communication/Communication.hpp"
#include "../subsystem/Sorter/Sorter.hpp"
#include "../subsystem/Intake/Intake.h"
#include "../subsystem/Elevator/Elevator.h"
#include "../subsystem/Dropper/Dropper.hpp"

extern Drive drive_;
extern DistanceSensor distance_sensor_;
extern LineSensor line_sensor_;
extern Communication com_;
extern Sorter sorter_;
extern Elevator elevator_;
extern Intake upperIntake_;
extern Intake lowerIntake_;
extern Intake rampIntake_;
extern Dropper dropper_;