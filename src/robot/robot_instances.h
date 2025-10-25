#pragma once

#include "../subsystem/Drive/Drive.hpp"
#include "../subsystem/DistanceSensors/DistanceSensor.hpp"
#include "../subsystem/LineSensor/LineSensor.hpp"
#include "../subsystem/Communication/Communication.hpp"
#include "../subsystem/Sorter/Sorter.hpp"
#include "../../lib/sensors/bno.hpp"
#include "./monitor.h"
#include "../subsystem/Intake/Intake.h"

extern Drive drive_;
extern DistanceSensor distance_sensor_;
extern LineSensor line_sensor_;
extern Communication com_;
extern Sorter sorter_;
// extern Elevator elevator_;
extern BNO bno_;
extern Monitor monitor_;
extern Intake intake_;