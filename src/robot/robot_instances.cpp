#include "robot/robot_instances.h"
#include "constants/constants.h"

using namespace Constants;

// Define global instances of robot subsystems
Drive drive_;
// Elevator elevator_;
Sorter sorter_;
LineSensor line_sensor_;
DistanceSensor distance_sensor_;
Communication com_;
BNO bno_;
SoftwareSerial bluetooth(0, 1);