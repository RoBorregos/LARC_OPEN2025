#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <Arduino.h>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"

using namespace Constants;

class Elevator : public System {
private:
    const int step_pin_;
    const int dir_pin_;
    
    const int kONStepCount = 200;

    int current_step_count_ = 0;
    
    // Method to move the elevator up or down depending on the goal step count
    void moveToStepCount(int goalSteps);
    
public:
    // Constructor with pin initialization
    Elevator();

    // Method to initialize the elevator system
    void begin() override;
    
    // Method to initialize the elevator system, move elevator to ON position
    bool setup();

    void update() override;
    void setState(int state) override;

    enum class ElevatorState {
        OFF = 0,
        ON = 1,
    };

    ElevatorState elevator_state_ = ElevatorState::OFF;
};

#endif // ELEVATOR_H