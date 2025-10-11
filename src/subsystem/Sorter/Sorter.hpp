#ifndef SORTER_H
#define SORTER_H

#include <Arduino.h>
#include "../systems/system.hpp"
#include "constants/constants.h"
#include "constants/pins.h"

using namespace Constants;

class Sorter : public System {
private:
    const int step_pin_;
    const int dir_pin_;
    
    bool is_running_;
    unsigned long last_step_time_;
    unsigned long step_delay_us_;
    
    int current_position_;
    int target_position_;
    
    static constexpr int POSITION_0 = 0;
    static constexpr int POSITION_180 = 200;
    
public:
    Sorter();

    void begin() override;
    void update() override;
    void setState(int state) override;

    void startStepper(int target_position, unsigned long speed_us = 1000);
    void stopStepper();
    void step(bool direction = true);
    bool isRunning() const;
    
    void moveToPosition0();
    void moveToPosition180();
    void moveToPosition(int position);
    
    int getCurrentPosition() const;
    
    void setDirection(bool forward = true);

    enum class SorterState {
        POSITION_0 = 0,
        POSITION_180 = 1,
        CUSTOM = 2
    };

    SorterState sorter_state_ = SorterState::POSITION_0;

private:
    void performStep();
};

#endif
