#include "robot/robot_instances.h"
#include "constants/constants.h"

using namespace Constants;

Monitor monitor_(0, 1);

Drive drive_;
Sorter sorter_;
LineSensor line_sensor_;
DistanceSensor distance_sensor_;
Elevator elevator_;
Communication com_;
Intake intake_;
BNO bno_;