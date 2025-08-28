/**
 * LARC 2025
 *
 * @Roborregos
 */

#include <Arduino.h>
#include <Wire.h>

#include "robot/robot_instances.h"

bool kill = false;

enum RobotState
{
  START,
  PASAR_LINEA,
  DETENER_Y_IZQUIERDA,
  ADELANTE_HASTA_LINEA,
  DETENER_IR_IZQUIERDA,
  FINAL
};

RobotState state = START;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  Wire.begin();

  // All systems must begin after initializing the serial and as the code starts running
  drive_.begin();
  elevator_.begin();
  com_.begin();

  drive_.setState(1);
  drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));
}

void loop()
{
  drive_.update();

  if (kill)
  {
    Serial.println("KILL");
    drive_.acceptInput(0, 0, 0);
    return;
  }

  // Lectura de sensores
  bool lineFL = line_sensor_.readSensor(Pins::kLineSensorFL);
  bool lineFR = line_sensor_.readSensor(Pins::kLineSensorFR);
  bool lineBL = line_sensor_.readSensor(Pins::kLineSensorBL);
  bool lineBR = line_sensor_.readSensor(Pins::kLineSensorBR);

  Serial.println("lineFL: " + String(lineFL) + " lineFR: " + String(lineFR) + " lineBL: " + String(lineBL) + " lineBR: " + String(lineBR));

  float left = distance_sensor_.getDistance(Pins::kLeftDistanceSensor);
  float right = distance_sensor_.getDistance(Pins::kRightDistanceSensor);
  bool obstacleFront = (left < 15) || (right < 15);

  delay(50);

  switch (state)
  {
    //   case START:
    //     Serial.println("ESTADO START");
    //     drive_.acceptInput(0, 100, 0);
    //     if (lineFL || lineFR)
    //     {
    //       delay(400);
    //       drive_.acceptInput(0, 0, 0);
    //       Serial.println("LINEA FRONTAL DETECTADA -> STOP");
    //       drive_.acceptInput(0, 0, 0);
    //       state = PASAR_LINEA;
    //       delay(1000);
    //     }
    //     break;

    //   case PASAR_LINEA:
    //     Serial.println("ESTADO PASAR_LINEA");
    //     drive_.acceptInput(150, 0, 0);
    //     if (obstacleFront) {
    //       drive_.acceptInput(0, 0, 100);
    //       delay(200);
    //       drive_.acceptInput(0, 0, 0);
    //       Serial.println("DETENERSE -> IR IZQUIERDA");
    //       state = DETENER_Y_IZQUIERDA;
    //       delay(500);
    //     } else if (lineFL || lineFR) {
    //       drive_.acceptInput(0, 0, 0);
    //       Serial.println("LINEA DETECTADA -> STOP");
    //       state = FINAL;
    //       kill = true;
    //     }
    //     break;

    // case DETENER_Y_IZQUIERDA:
    //   Serial.println("ESTADO DETENER_Y_IZQUIERDA");
    //   drive_.acceptInput(-150, 0, 0);
    //   delay(400);
    //   drive_.acceptInput(0, 0, 0);
    //   state = ADELANTE_HASTA_LINEA;
    //   break;

    // case ADELANTE_HASTA_LINEA:
    //   Serial.println("ESTADO ADELANTE_HASTA_LINEA");
    //   drive_.acceptInput(0, 150, 0);
    //   if (lineFL && lineFR) {
    //     drive_.acceptInput(0, 0, 0);
    //     Serial.println("DETENERSE -> IR  IZQUIERDA");
    //     state = DETENER_IR_IZQUIERDA;
    //     delay(300);
    //   }
    //   break;

    // case DETENER_IR_IZQUIERDA:
    //   Serial.println("ESTADO DETENER_IR_IZQUIERDA");
    //   drive_.acceptInput(-150, 0, 0);
    //   delay(300);
    //   drive_.acceptInput(0, 0, -150);
    //   delay(300);
    //   drive_.acceptInput(0, 0, 0);
    //   state = FINAL;
    //   break;

    // case FINAL:
    //   Serial.println("ESTADO FINAL");
    //   if (lineFL && lineBL) {
    //     drive_.acceptInput(0, 0, 0);
    //     Serial.println("LINEA FINAL DETECTADA -> STOP");
    //     kill = true;
    //   } else {
    //     drive_.acceptInput(0, 150, 0);
    //   }
    //   break;
  }
}

// /**
//  * LARC 2025
//  *
//  * @Roborregos
//  */

//  #include <Arduino.h>
//  #include <Wire.h>

//  #include "robot/robot_instances.h"

//  bool line_crossed = false;
//  bool horizontal_line_found = false;
//  bool first_time = true;
//  bool kill = false;

//  void setup()
//  {
//    Serial.begin(9600);
//    Serial.println("Starting...");
//    Wire.begin();

//    // All systems must begin after initializing the serial and as the code starts running
//    drive_.begin();
//    elevator_.begin();
//    com_.begin();

//    drive_.setState(0),
//        drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));
//  }

//  void loop()
//  {
//    drive_.update();
//    // line_sensor_.readSensor(Pins::kLineSensorFL);
//    // line_sensor_.readSensor(Pins::kLineSensorFR);
//    // delay(500);
//    // delay(500);
//    // line_sensor_.readSensor(Pins::kRightDistanceSensor);
//    // Va para enfrente

//    if (kill)
//    {
//      drive_.acceptInput(0, 0, 0);
//      return;
//    }

//    if (!line_crossed)
//    {
//      drive_.acceptInput(0, 100, 0);
//      if (line_sensor_.readSensor(Pins::kLineSensorFR) ||
//          line_sensor_.readSensor(Pins::kLineSensorFL))
//      {
//        line_crossed = true;
//      }
//    }
//    else
//    {
//      if (first_time)
//      {
//        drive_.acceptInput(0, 0, 0);
//        Serial.println("STOPPED LINE");
//        first_time = false;
//        delay(400);
//      }
//      else
//      {
//        if (horizontal_line_found)
//        {
//          drive_.acceptInput(0, 0, 0);
//          Serial.println("STOPPED HORIZONTAL");
//          kill = true;
//          delay(400);
//        }
//        else
//        {
//          drive_.acceptInput(200, 0, 0);
//          Serial.println("MOVING");
//        }

//        if (line_sensor_.readSensor(Pins::kLineSensorFR))
//        {
//          horizontal_line_found = true;
//        }
//      }
//    }
//  }