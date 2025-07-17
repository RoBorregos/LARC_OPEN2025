#pragma once

#include "PID_v1.h"
#include "robot_instances.h"
#include "../lib/math/pose2d.hpp"
#include <FreeRTOS.h>
#include <task.h>
#include "../include/constants/constants.h"

using namespace Constants;

TaskHandle_t globalUpdateTaskHandle;

void setupTasks();

void globalUpdateTask(void *pvParameters);