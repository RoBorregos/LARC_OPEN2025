#pragma once

#include "../subsystem/Drive/Drive.hpp"
#include "../subsystem/Elevator/Elevator.hpp"
#include "../subsystem/Gripper/Gripper.hpp"
#include "../subsystem/LowerSorter/LowerSorter.hpp"
#include "../subsystem/UpperSorter/UpperSorter.hpp"
#include "../subsystem/LineSensor/LineSensor.h"
#include "../subsystem/Vision/Camera.hpp"
#include "PID_v1.h"

extern Drive drive_;
extern Elevator elevator_;
extern Gripper gripper_;
extern LowerSorter lower_sorter_;
extern UpperSorter upper_sorter_;
extern PIDController centerPID_;
extern LineSensor line_sensor_;
extern Camera camera_;

bool centerWithObject(double elapsed_time);
bool alignWithObject(double elapsed_time, float desired_distance);

bool initStart();
bool exitStart(double elapsed_time);

bool goTreeZone(double elapsed_time);
bool goStorageZone(double elapsed_time);

bool avoidPool(double elapsed_time);

bool goLeftLimit(double elapsed_time);
bool goRightLimit(double elapsed_time);

bool searchForTrees(double elapsed_time, bool direction);
bool goStorage(double elapsed_time, int container_type);

bool dropBeans(double elapsed_time, int container_type);

bool pickBean(double elapsed_time, int level);

bool globalUpdate();
