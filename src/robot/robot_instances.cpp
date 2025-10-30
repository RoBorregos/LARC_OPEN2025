#include "robot/robot_instances.h"
#include "constants/constants.h"

using namespace Constants;

Drive drive_;
Sorter sorter_;
LineSensor line_sensor_;
DistanceSensor distance_sensor_;
Elevator elevator_;
Communication com_;
Intake upperIntake_(Pins::kUpperIntakeServo);
Intake lowerIntake_(Pins::kLowerIntakeServo);
Intake rampIntake_(Pins::kRampIntakeServo);
BNO bno_;
Dropper dropper_;