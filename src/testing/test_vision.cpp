#include <Arduino.h>
#include "../robot/robot_instances.h"
 
void setup(){
    Serial.begin(9600);
    upperIntake_.begin();
    lowerIntake_.begin();
    rampIntake_.begin();
    com_.begin();
}

void loop(){

    com_.update();
    upperIntake_.update();
    lowerIntake_.update();
    rampIntake_.update();

    static bool visionReady = false;
    static int lastTop = -1;
    static int lastBottom = -1;

    string cmd = com_.getCommand();
    Serial.println("PICKUP STATE");

    if (!visionReady)
    {
    if (cmd == "XAVIER RUNNING VISION")
    {
        visionReady = true;
        Serial.println("[INFO] Xavier vision online — starting movement");
    }
    else
    {
        drive_.acceptInput(0, 0, 0);
        return;
    }
    }

    auto values = com_.getMatrix();
    int top = values[0];
    int bottom = values[1];

    if (top != -1 && top != lastTop)
    {
        if (top == 2 || top == 1)
        {
        upperIntake_.setState(1);
        }
        else if (top == 0)
        {
        upperIntake_.setState(0);
        }
        lastTop = top;
    }

    if (bottom != -1 && bottom != lastBottom)
    {
        if (bottom == 2 || bottom == 1)
        {
        lowerIntake_.setState(1);
        }
        else if (bottom == 0)
        {
        lowerIntake_.setState(0);
        }
        lastBottom = bottom;
    }
}