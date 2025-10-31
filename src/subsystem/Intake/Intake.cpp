#include "Intake.h"

Intake::Intake(int pin, int storedPosition_, int grabPosition_)
    : servo(),
      pin(pin), grabPosition(grabPosition_), storedPosition(storedPosition_)
{
}

void Intake::begin()
{
    servo.attach(pin);

    update();
}

void Intake::update()
{
}

void Intake::setState(int state)
{
    IntakeState newState = static_cast<IntakeState>(state);
    if (newState == intake_state_)
        return; // no change — don’t rewrite servo signal

    intake_state_ = newState;

    // Only update servo when state actually changes
    switch (intake_state_)
    {
    case IntakeState::STORED:
        Serial.print("Moved servo to STORED - PIN: " + String(pin));
        setIntakeServoPosition(storedPosition);
        break;
    case IntakeState::POSITIONED:
        Serial.print("Moved servo to POSITIONED - PIN: " + String(pin));
        setIntakeServoPosition(grabPosition);
        break;
    }
}

void Intake::setIntakeServoPosition(int position)
{
    servo.write(position);
}
