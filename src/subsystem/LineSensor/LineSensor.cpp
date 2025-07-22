/*
 * @file LineSensor.cpp
 * @date 21/07/2025
 * @author Hector Tovar
 *
 * @brief Cpp file of the Almacen class to control two servos.
 */

#include "LineSensor.hpp"
#include <cstdlib>

LineSensor::LineSensor() {}

bool LineSensor::leftDetected() {
  bool value = digitalRead(leftPin);
  return value;
}

bool LineSensor::rightDetected() {
  bool value = digitalRead(rightPin);
  return value;
}