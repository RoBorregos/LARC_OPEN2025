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

    elevator_.setState(2); // 2 TO DOWN 1 FOR UP
    elevator_.update();
    delay(32000);
}

void loop(){
}