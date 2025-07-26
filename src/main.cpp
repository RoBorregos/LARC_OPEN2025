/**
 * LARC 2025
 *
 * @Roborregos
 */

#include <Arduino.h>
#include <Wire.h>
#include <FreeRTOS.h>
#include <task.h>

#include "robot/tasks.h"

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  drive_.moveForward(0);

  // // FreeRTOS setup
  setupTasks();

  // Start RTOS scheduler
  vTaskStartScheduler();
}

// This is not used in RTOS
void loop()
{
}
