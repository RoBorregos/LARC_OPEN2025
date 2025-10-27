#ifndef SORTER_H
#define SORTER_H

#include <Arduino.h>
#include <AccelStepper.h>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"

using namespace Constants;


class Sorter : public System {
private:
    AccelStepper stepper;
    static constexpr uint8_t kMotorInterfaceType = 1; // DRIVER
    static constexpr uint16_t kMaxStepperSpeed = 2000;
    static constexpr uint16_t kStepperAcceleration = 700;
    static constexpr int kStepsPerPosition = 200; // steps between consecutive preset positions

    // unsigned long last_step_time_;
    // unsigned long step_delay_us_;
    
    // int current_position_;
    // int target_position_;
    
    // static constexpr int POSITION_0 = 0;
    // static constexpr int POSITION_180 = 200;
    
public:
    enum class SorterState {
        MATURE_POS_1 = 0,
        MATURE_POS_2 = 1,
        MATURE_POS_3 = 2,
        OVERMATURE_POS_1 = 3,
        OVERMATURE_POS_2 = 4,
        OVERMATURE_POS_3 = 5,
    };

    Sorter();

    void begin() override;
    void update() override;
    void setState(int state) override;

    void setMaxSpeed(uint16_t speed);
    void setAcceleration(uint16_t acceleration);
    void setSpeed(uint16_t speed);

    // Move to one of the six preset logical states (uses kStepsPerPosition to map to steps)
    void moveToState(SorterState target_state);

    bool isRunning();
    // void startStepper(int target_position, unsigned long speed_us = 1000);
    // void stopStepper();
    // void step(bool direction = true);
    // bool isRunning() const;
    
    // void moveToPosition0();
    // void moveToPosition180();
    // void moveToPosition(int position);
    
    // int getCurrentPosition() const;
    
    // void setDirection(bool forward = true);

    SorterState sorter_state_ = SorterState::MATURE_POS_1;

};

#endif
