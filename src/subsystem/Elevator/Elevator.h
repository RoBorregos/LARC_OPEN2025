/**
 * Written by Juan Pablo Gutierrez
 *
 * 26 10 2024
 */

#pragma once
#include "../systems/system.hpp"

class Elevator : public System
{
public:
    Elevator();
    void begin();
    void update();
    void setState(int state);

private:
    int elevator_state_;
};