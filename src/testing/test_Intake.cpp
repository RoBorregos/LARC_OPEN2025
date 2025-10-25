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
    Serial.println("Setting intake to STORED position");
    intake_.update();
    delay(2000);
    intake_.setState(1);
    Serial.println("Setting intake to ALL POSITIONED position");
    intake_.update();
    delay(2000);
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
