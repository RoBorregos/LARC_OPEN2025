#include <Arduino.h>
#include <Wire.h>
#include "../robot/robot_instances.h"

void setup(){
    elevator_.begin();
    lowerIntake_.begin();
    upperIntake_.begin();
    rampIntake_.begin();

    lowerIntake_.setState(0);
    upperIntake_.setState(0);
    rampIntake_.setState(0);
}

void loop(){
    elevator_.setState(2);
    delay(32000);
}