#include <Arduino.h>
#include "../robot/robot_instances.h"

Intake intake;

void setup(){
    Serial.begin(9600);
    intake.begin();

   
}

void loop(){
    // intake_.update();
    // intake_.setServoPosition(intake_.UpperIntakeServo, 0);
    // delay(2000);
    // intake_.setServoPosition(intake_.UpperIntakeServo, 80);
    // delay(2000);

    intake_.LowerIntakeServo.write(180);
    // Empty loop
}