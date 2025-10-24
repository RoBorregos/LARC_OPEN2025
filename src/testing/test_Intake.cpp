#include <Arduino.h>
#include "../robot/robot_instances.h"

Intake intake;

void setup()
{
    Serial.begin(9600);
    intake_.begin();
}

void loop()
{
    intake_.setState(0);
    intake_.update();
    delay(2000);
    intake_.setState(1);
    intake_.update();
    delay(2000);
}
