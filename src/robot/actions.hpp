#pragma once

#include "PID_v1.h"
#include "robot_instances.h"
#include "../lib/math/pose2d.hpp"

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
