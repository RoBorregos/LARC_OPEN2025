#include <Arduino.h>
#include "../robot/robot_instances.h"

Intake intake;

void setup(){
    Serial.begin(9600);
    intake_.begin();
}

void loop(){
    // intake_.update();
    intake_.setIntakeServoPosition(intake_.LowerIntakeServo, 180);
    delay(2000);
    intake_.setIntakeServoPosition(intake_.LowerIntakeServo, 0);
    delay(2000);
}


/////////////////////////// No Repo /////////////////////////////////////////


// #include <Arduino.h>
// #include <Servo.h>

// Servo servo;

// void setup(){
//     Serial.begin(9600);
//     servo.attach(3);
// }

// void loop(){
//     servo.write(180);
//     delay(2000);
//     servo.write(0);
//     delay(2000);
// }