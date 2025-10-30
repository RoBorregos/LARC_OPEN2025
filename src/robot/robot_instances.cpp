#include "robot/robot_instances.h"
#include "constants/constants.h"

using namespace Constants;

Drive drive_;
Sorter sorter_;
LineSensor line_sensor_;
DistanceSensor distance_sensor_;
Elevator elevator_;
Communication com_;
Intake upperIntake_(Pins::kUpperIntakeServo, IntakeConstants::kAvoidBallUpperServoPosition, IntakeConstants::kGrabBallUpperServoPosition);
Intake lowerIntake_(Pins::kLowerIntakeServo, IntakeConstants::kAvoidBallLowerServoPosition, IntakeConstants::kGrabBallLowerServoPosition);
Intake rampIntake_(Pins::kRampIntakeServo, IntakeConstants::kIntakeRampStored, IntakeConstants::kIntakeRampPositioned);
BNO bno_;
Dropper dropper_;