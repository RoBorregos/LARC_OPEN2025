/**
 * LARC 2025
 *
 * @Roborregos
 */

#include <Arduino.h>
#include <Wire.h>

#include "robot/robot_instances.h"

bool kill = false;
unsigned long startTime = 0;

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

  // TEST CASE LINE
  bool lineFL = line_sensor_.readSensor(Pins::kLineSensorFL);
  bool lineFR = line_sensor_.readSensor(Pins::kLineSensorFR);
  bool lineBL = line_sensor_.readSensor(Pins::kLineSensorBL);
  bool lineBR = line_sensor_.readSensor(Pins::kLineSensorBR);
  Serial.println(String(lineFL) + String(lineFR) + String(lineBL) + String(lineBR));

  delay(1000);
  
  // TEST CASE DISTANCE
  float frontLeft = distance_sensor_.getDistance(0);
  float frontRight = distance_sensor_.getDistance(1);
  float backLeft = distance_sensor_.getDistance(2);
  float backRight = distance_sensor_.getDistance(3);
  Serial.println(String(frontLeft) + String(frontRight) + String(backLeft) + String(backRight));

  // STATE
  bool obstacleFront = (frontLeft < 15) || (frontRight < 15);
  
  delay(1000);
  
  // delay(50);
  
  // switch (state)
  // {
  //     case START:
  //       Serial.println("ESTADO START");

  //       if (startTime == 0) {
  //         startTime = millis();
  //       }

  //       drive_.acceptInput(0, 150, 0);

  //       if (millis() - startTime > 1500) {
  //         if (obstacleFront) {
  //           drive_.acceptInput(0, 0, 0);
  //           delay(200);
  //           Serial.println("DETENERSE -> IR IZQUIERDA");
  //           state = DETENER_Y_IZQUIERDA;
  //           startTime = 0;
  //         } else if (lineFL && lineFR) {
  //           drive_.acceptInput(0, 0, 0);
  //           Serial.println("LINEA DETECTADA -> STOP");
  //           state = FINAL;
  //           kill = true;
  //           startTime = 0;
  //         }
  //       }
  //       break;
          
  //     case DETENER_Y_IZQUIERDA:
  //       Serial.println("ESTADO DETENER_Y_IZQUIERDA");
  //       drive_.acceptInput(-150, 0, 0);
  //       delay(1500);
  //       drive_.acceptInput(0, 0, 0);
  //       state = FINAL;
  //       break;

  //     case FINAL:
  //       Serial.println("ESTADO FINAL");
  //       if (lineFL && lineBL) {
  //         drive_.acceptInput(0, 0, 0);
  //         Serial.println("LINEA FINAL DETECTADA -> STOP");
  //         kill = true;
  //       } else if(lineFL && lineFR){
  //         drive_.acceptInput(0,-150,0);
  //         delay(1000);
  //         drive_.acceptInput(-150,0,0);
  //       } 
  //       else {
  //         drive_.acceptInput(0, 150, 0);
  //       }
  //       break;
  // }
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