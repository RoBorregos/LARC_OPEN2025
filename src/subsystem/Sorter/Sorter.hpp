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
    static constexpr uint16_t kMaxStepperSpeed = 200;
    static constexpr uint16_t kStepperAcceleration = 15000;
    static constexpr int kStepsPerPosition = 273; // steps between consecutive preset positions
    void setDirection(bool forward);

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
    void moveToState(SorterState target_state);
    bool isRunning();
    void stepperHardBrake();

    SorterState sorter_state_ = SorterState::MATURE_POS_1;

};

#endif
