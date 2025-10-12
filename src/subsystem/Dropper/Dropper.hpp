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
    Servo servo_;
    const int servo_pin_;
    
    static constexpr int POSITION_0 = 0;
    static constexpr int POSITION_180 = 180;
    
    int current_position_;
    
public:
    Dropper();

    void begin() override;
    void update() override;
    void setState(int state) override;

    void setPosition(int position);
    void setTo0();
    void setTo180();
    
    int getCurrentPosition() const;
    
    bool isAt0() const;
    bool isAt180() const;

    enum class DropperState {
        POSITION_0 = 0,
        POSITION_180 = 1
    };

    DropperState dropper_state_ = DropperState::POSITION_0;
};

#endif // DROPPER_H
