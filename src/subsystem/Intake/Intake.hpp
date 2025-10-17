#ifndef INTAKE_H
#define INTAKE_H

#include <Arduino.h>
#include <Servo.h>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"

using namespace Constants;

class Intake : public System {
private:
    Servo servo1_;
    Servo servo2_;
    Servo servo3_;
    
    const int servo1_pin_;
    const int servo2_pin_;
    const int servo3_pin_;
    
    static constexpr int POSITION_0 = 0;
    static constexpr int POSITION_180 = 180;
    
    int servo1_position_;
    int servo2_position_;
    int servo3_position_;
    
public:
    Intake();

    void begin() override;
    void update() override;
    void setState(int state) override;

    void setServo1Position(int position);
    void setServo2Position(int position);
    void setServo3Position(int position);
    
    void setServo1To0();
    void setServo1To180();
    void setServo2To0();
    void setServo2To180();
    void setServo3To0();
    void setServo3To180();
    
    int getServo1Position() const;
    int getServo2Position() const;
    int getServo3Position() const;
    
    void setAllServosTo0();
    void setAllServosTo180();
    void setAllServosToPosition(int position);

    enum class IntakeState {
        ALL_0 = 0,
        ALL_180 = 1,
        CUSTOM = 2
    };

    IntakeState intake_state_ = IntakeState::ALL_0;
};

#endif // INTAKE_H