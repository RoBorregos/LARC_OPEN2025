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
#include "tasks.h"
#include "robot_instances.h"
class StateManager {
private:

    RobotState global_state_;

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
