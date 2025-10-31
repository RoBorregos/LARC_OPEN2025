#include "Intake.h"

Intake::Intake(int pin, int storedPosition_, int grabPosition_, int avoidPosition_)
    : servo(),
      pin(pin), grabPosition(grabPosition_), storedPosition(storedPosition_), avoidPosition(avoidPosition_)
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
    case IntakeState::AVOID:
        Serial.print("Moved servo to AVOID - PIN: " + String(pin));
        setIntakeServoPosition(avoidPosition);
        break;
    }
}

void Intake::setIntakeServoPosition(int position)
{
    servo.write(position);
}
