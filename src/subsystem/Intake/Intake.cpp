#include "Intake.hpp"

Intake::Intake()
{
    upper.attach(Pins::kUpperServo);
    middle.attach(Pins::kMiddleServo);
    lower.attach(Pins::kLowerServo);

    delay(100);
}

void Intake::update()
{
    switch (intake_state)
    {
    case IntakeState::COLLECT:
        move(IntakeConstants::kCollect);
        break;
    case IntakeState::IDLE:
        move(IntakeConstants::kIdle);
        break;
    }
}

void Intake::setState(int state)
{
    intake_state = static_cast<IntakeState>(state);
}

void Intake::move(int angle)
{
    upper.write(angle);
    delay(1500);
    middle.write(angle);
    delay(1500);
    lower.write(angle);
    delay(1500);
}