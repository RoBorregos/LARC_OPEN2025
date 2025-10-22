#ifndef DROPPER_H
#define DROPPER_H

#include <Arduino.h>
#include <Servo.h>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"

using namespace Constants;

class Dropper : public System {
private:

    static constexpr int CLOSED_POSITION = 0;
    static constexpr int OPEN_POSITION = 180;

    int current_position_;
    
public:

    Servo dropper_servo_;

    Dropper();

    void begin() override;
    void update() override;
    void setState(int state) override;
    void setDropperPosition(int position);
    int getDropperPosition();
    
    enum class DropperState {
        DROPPER_CLOSED = 0,
        DROPPER_OPEN = 1
    };

    DropperState dropper_state_ = DropperState::DROPPER_CLOSED;
};

#endif // DROPPER_H
