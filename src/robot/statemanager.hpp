/*
 * @file StateManager.hpp
 * @date 01/04/2025
 * @author  Dana E. Torres Estrada
 *
 * @brief Header file for the StateManager class, which manages the robot's state and subsystems.
*/
#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include <Arduino.h>
#include "RobotState.h"
#include "../subsystem/Elevator/Elevator.hpp"
#include "actions.hpp"
class StateManager {
private:

    RobotState state_;

    const int mid_level_beans_ = 4; // REGO //2 para ROBO
    const int low_level_beans_ = 3; // REGO 3 o 2 // 1 para ROBO

    // Control variables
    long start_time_ = 0;
    bool direction_ = true; // 0 or false = left | 1 or true = right
    int current_tree_ = 0;
    int current_beans_ = 0;
    bool droped_SOBREMADURO = false; // 0 or MADURO = left | 1 or SOBREMADURO
    long state_start_time_ = 0; 

public:
    StateManager();
    void stateAction();
    void stateTransition();
    void setState(RobotState state);
    RobotState getState();
    unsigned long getTimeSpent();
    void update();
};

#endif // STATE_MANAGER_HPP
