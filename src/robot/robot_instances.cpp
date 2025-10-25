#include "robot/robot_instances.h"
#include "constants/constants.h"

using namespace Constants;

Drive drive_;
Sorter sorter_;
LineSensor line_sensor_;
DistanceSensor distance_sensor_;
Communication com_;
BNO bno_;
Monitor monitor_(0, 1);
Intake intake_;