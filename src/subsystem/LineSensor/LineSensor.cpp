/*
 * @file LineSensor.cpp
 * @date 07/05/2025
 * @author Brisma Alvarez Valdez
 *
 * @brief Cpp file of the Almacen class to control two servos.
 */

#include "LineSensor.h"
#include <cstdlib>

/*LineSensor::LineSensor() {
  this->leftPin = Pins::kLineSensorLeftPin;
  this->rightPin = Pins::kLineSensorRightPin;
}

void LineSensor::begin() {
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
}

bool LineSensor::leftDetected() {
  return digitalRead(leftPin) == HIGH;
}

bool LineSensor::rightDetected() {
  return digitalRead(rightPin) == HIGH;
}*/

LineSensor::LineSensor() {}

void LineSensor::update() {
  String received = Serial.readStringUntil('\n');
  received.trim();
  
  int leftPos = received.indexOf("Left:");
  int rightPos = received.indexOf("Right:");
  
  if (leftPos != -1) {
    String leftValue = received.substring(leftPos + 5, rightPos != -1 ? rightPos : received.length());
    leftValue = leftValue.trim();
    bool new_left_detected = (leftValue == "YES");
    if (new_left_detected && !left_detected_) {
      left_detection_time_ = millis();
    }
    left_detected_ = new_left_detected;
  }
  
  if (rightPos != -1) {
    String rightValue = received.substring(rightPos + 6);
    rightValue = rightValue.trim();
    bool new_right_detected = (rightValue == "YES");
    if (new_right_detected && !right_detected_) {
      right_detection_time_ = millis();
    }
    right_detected_ = new_right_detected;
  }
}

bool LineSensor::leftDetected() {
  return left_detected_;
}

bool LineSensor::rightDetected() {
  return right_detected_;
}

bool LineSensor::bothDetectedWithin500ms() {
  if (!left_detected_ || !right_detected_) return false;
  return std::abs(static_cast<long>(left_detection_time_ - right_detection_time_)) <= 500;
}

