#include <Arduino.h>
#include "constants/pins.h"

void setup()
{
    Serial.begin(9600);

    pinMode(Pins::kElevatorINA1, OUTPUT);
    pinMode(Pins::kElevatorINA2, OUTPUT);
    pinMode(Pins::kElevatorPWM, OUTPUT);
}

void goUp()
{
    digitalWrite(Pins::kElevatorINA1, HIGH);
    digitalWrite(Pins::kElevatorINA2, LOW);
    analogWrite(Pins::kElevatorPWM, 255); // Set PWM value for speed
}

void goDown()
{
    digitalWrite(Pins::kElevatorINA1, LOW);
    digitalWrite(Pins::kElevatorINA2, HIGH);
    analogWrite(Pins::kElevatorPWM, 255); // Set PWM value for speed
}

void loop()
{
    goUp();
    delay(10000); // Move for 10 seconds
    goDown();
    delay(10000); // Move for 10 seconds

}
