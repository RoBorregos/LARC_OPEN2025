#include <Arduino.h>
#include "../robot/robot_instances.h"

void setup()
{
    Serial.begin(9600);
    upperIntake_.begin();
    lowerIntake_.begin();
    rampIntake_.begin();
}

void loop()
{
    // Open all intakes
    upperIntake_.setState(1);
    lowerIntake_.setState(1);
    rampIntake_.setState(1);
    delay(2000);

    // Close all intakes
    upperIntake_.setState(0);
    lowerIntake_.setState(0);
    rampIntake_.setState(0);
    delay(2000);
}

////////////////////////////// No Repo functions Test //////////////////////////////

// #include <Arduino.h>
// #include <Servo.h>
// Servo servoUp;
// Servo servoDown;
// Servo servoRamp;

// void setup(){
//   servoUp.attach(3);
//   servoDown.attach(6);
//   servoRamp.attach(4);
// }

// void loop(){
//   servoDown.write(125);
//   delay(500);
//   servoUp.write(150);
//   delay(500);
// }
