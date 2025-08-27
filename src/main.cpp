/**
 * LARC 2025
 *
 * @Roborregos
 */

#include <Arduino.h>
#include <Wire.h>

#include "robot/robot_instances.h"

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  Wire.begin(); 

  // All systems must begin after initializing the serial and as the code starts running
  drive_.begin();
  elevator_.begin();
  com_.begin();

  drive_.setState(0),
  drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));
}

void loop() {
  drive_.update();
  delay(50);

  // Va para enfrente
  drive_.acceptInput(0, 200, 0);

  // Si detecta una linea
  if (line_sensor_.readSensor(Pins::kLineSensorFR) || 
      line_sensor_.readSensor(Pins::kLineSensorFL)) {
    // La pasa por ms y luego se detiene 
    delay(3000);
    drive_.acceptInput(0, 0, 0);

    bool leftClear  = distance_sensor_.getDistance(Pins::kLeftDistanceSensor)  >= 15;
    bool rightClear = distance_sensor_.getDistance(Pins::kRightDistanceSensor) >= 15;

    if (leftClear && rightClear) {
      drive_.acceptInput(200, 0, 0);
      return;
    }

    drive_.acceptInput(0, 200, 0);

    if (line_sensor_.readSensor(Pins::kLineSensorFR) || 
        line_sensor_.readSensor(Pins::kLineSensorFL)) {
      
      drive_.acceptInput(200, 0, 0);

      if (line_sensor_.readSensor(Pins::kLineSensorFR) || 
          line_sensor_.readSensor(Pins::kLineSensorBR)) {
        drive_.acceptInput(0, 0, 0);
      }
    }
  }
}