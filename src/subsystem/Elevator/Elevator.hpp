#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <Arduino.h>
#include "../systems/system.hpp"
#include "constants/constants.h"

using namespace Constants;

class Elevator : public System {
private:
    const int step_pin_;
    const int dir_pin_;
    
    float current_height_ = 0.0; // in cm
    
    // Method to convert a distance in centimeters to the corresponding number of steps. Returns number of steps required to move the given distance
    int translateCmToSteps(float distance);
    
public:
    // Constructor with initialization list
    Elevator(int leftStepPin, int leftDirPin, int rightStepPin, int rightDirPin);

    // Method to initialize the elevator system
    bool setup();

    // Method to move the elevator up by a specified distance in cm
    void moveUp(float distance);

    // Method to move the elevator down by a specified distance in cm
    void moveDown(float distance);

    // Method to move to a specific height in cm
    void moveToHeight(float height);

    // Method to stop the motors
    void stop();

    void update() override;
    void setState(int state) override;

    enum class ElevatorState {
        HOME = 0,
        LOW_POSITION = 1,
        MID_POSITION = 2,
        HIGH_POSITION = 3,
    };

    ElevatorState elevator_state_ = ElevatorState::HOME;
};

#endif // ELEVATOR_H