#include "Intake.h"

Intake::Intake(int pin, int grabPosition_, int storedPosition_)
    : servo(),
      pin(pin), grabPosition(grabPosition_), storedPosition(storedPosition_)
{
}

void Intake::begin()
{
    servo.attach(pin);

    setState(1);
    update();
}

void Intake::update()
{
    switch (intake_state_)
    {
    case IntakeState::STORED:
        setIntakeServoPosition(storedPosition);
        break;
    case IntakeState::POSITIONED:
        setIntakeServoPosition(grabPosition);
        break;
    default:

        break;
    }
}

void Intake::setState(int state)
{
    intake_state_ = static_cast<IntakeState>(state);
}

void Intake::setIntakeServoPosition(int position)
{
    servo.write(position);
}
