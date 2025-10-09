#ifndef S0RTER_H
#define S0RTER_H

#include <Arduino.h>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"

using namespace Constants;

class Elevator : public System {
private:
    const int motor_pin_;
    
public:
    Elevator();

    void begin() override;

    void update() override;
    void setState(int state) override;

    void activateMotor();
    void deactivateMotor();
    bool isActive() const;

    enum class ElevatorState {
        INACTIVE = 0,
        ACTIVE = 1,
    };

    ElevatorState elevator_state_ = ElevatorState::INACTIVE;
};

#endif // S0RTER_H