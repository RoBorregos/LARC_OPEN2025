#include <Arduino.h>
#include "../robot/robot_instances.h"


void setup()
{
    Serial.begin(9600);
    intake_.begin();
    // dropper_.begin();
}

void loop()
{
    // dropper_.setState(0);
    // dropper_.update();
    // delay(2000);
    // dropper_.setState(1);
    // dropper_.update();
    // delay(2000);

    // intake_.setState(0);
    // Serial.println("Setting intake to STORED position");
    // intake_.update();
    // delay(2000);
    // intake_.setState(1);
    // Serial.println("Setting intake to ALL POSITIONED position");
    // intake_.update();
    // delay(2000);
    intake_.setState(2);
    Serial.println("Setting intake to UPPER SERVO ONLY STORED position");
    intake_.update();
    delay(2000);
    intake_.setState(3);
    Serial.println("Setting intake to LOWER SERVO ONLY STORED position");
    intake_.update();
    delay(2000);
    intake_.setState(4);
    Serial.println("Setting intake to UPPER SERVO ONLY POSITIONED position");
    intake_.update();
    delay(2000);
    intake_.setState(5);
    Serial.println("Setting intake to LOWER SERVO ONLY POSITIONED position");
    intake_.update();
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




