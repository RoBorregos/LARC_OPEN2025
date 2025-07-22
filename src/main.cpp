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

  // FreeRTOS setup
  setupTasks();

  // Start RTOS scheduler
  vTaskStartScheduler();
}

// This is not used in RTOS
void loop()
{
}
