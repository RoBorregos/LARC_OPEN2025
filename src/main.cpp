/**
 * LARC 2025
 *
 * @Roborregos
 */

#include <Arduino.h>
#include <Wire.h>

#include "robot/robot_instances.h"

bool line_crossed = false;
bool horizontal_line_found = false;
bool first_time = true;
bool kill = false;

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

void loop()
{
  drive_.update();

  if (kill)
  {
    drive_.acceptInput(0, 0, 0);
    return;
  }

  bool lineFL = line_sensor_.readSensor(Pins::kLineSensorFL);
  bool lineFR = line_sensor_.readSensor(Pins::kLineSensorFR);
  bool lineBL = line_sensor_.readSensor(Pins::kLineSensorBL);
  bool lineBR = line_sensor_.readSensor(Pins::kLineSensorBR);

  bool obstacleLeft  = distance_sensor_.getDistance(Pins::kLeftDistanceSensor)  < 10;
  bool obstacleRight = distance_sensor_.getDistance(Pins::kRightDistanceSensor) < 10;
  bool obstacleFront = obstacleLeft || obstacleRight;

  if (lineFL && lineBL)
  {
    drive_.acceptInput(0, 0, 0);
    Serial.println("LINEA FINAL DETECTADA -> IR DERECHA");
    delay(300);

    drive_.acceptInput(0, -150, 0); 
    delay(400);

    drive_.acceptInput(200, 0, 0); 
    delay(400);

    drive_.acceptInput(0, 0, 0); 
    kill = true;
  }
  else if (lineFL || lineFR)
  {
    drive_.acceptInput(-200, 0, 0);
    Serial.println("LINEA DETECTADA -> IZQUIERDA");
  }
  else if (obstacleFront)
  {
    drive_.acceptInput(200, 0, 0);
    Serial.println("OBSTACULO DETECTADO -> DERECHA");
  }
  else
  {
    drive_.acceptInput(0, 200, 0);
    Serial.println("ADELANTE");
  }
}
