#include "robot/robot_instances.h"
#include "constants/constants.h"

using namespace Constants;

// Define global instances of robot subsystems
Drive drive_;
Elevator elevator_;
Gripper gripper_;
LineSensor line_sensor_;
LowerSorter lower_sorter_;
UpperSorter upper_sorter_;
// Camera camera_;
// PIDController centerPID_(VisionConstants::kCenterPIDKp, VisionConstants::kCenterPIDKi, VisionConstants::kCenterPIDKd, 
//                         VisionConstants::kCenterPIDOutputMin, VisionConstants::kCenterPIDOutputMax);
// LineSensor line_sensor_; 