/*
 * @file statemanager.cpp
 * @date 01/04/2025
 * @author Juan Pablo Gutiérrez & Dana E. Torres Estrada
 *
 * @brief Implementation file for the StateManager class, which manages the robot's state and subsystems.
 */

#include "statemanager.hpp"

StateManager::StateManager() {}

void StateManager::setState(RobotState state)
{
    global_state_ = state;
}

RobotState StateManager::getState()
{
    return global_state_;
}

void StateManager::stateTransition()
{
    switch (global_state_)
    {
    default:
        break;
    }
}

void StateManager::update()
{

    // Execute the current state's action and get completion status
    switch (global_state_)
    {
    case RobotState::INIT:
    {
        break;
    }

    /*case RobotState::EXIT_START: {
        action_completed = exitStart(getTimeSpent());
        break;
    }
    case RobotState::GO_TREES: {
        action_completed = goTreeZone(getTimeSpent());
        break;
    }
    case RobotState::AVOID_POOL: { //TODO:
        action_completed = avoidPool(getTimeSpent());
        break;
    }
    case RobotState::GO_LEFT_LINE: {
        action_completed = goLeftLimit(getTimeSpent());
        break;
    }
    case RobotState::SEARCH_TREES: {
        action_completed = searchForTrees(getTimeSpent(), direction_);
        break;
    }
    case RobotState::PICK_MID_LEVEL: {
        action_completed = pickBean(getTimeSpent(), 2);
        break;
    }
    case RobotState::PICK_LOW_LEVEL: {
        action_completed = pickBean(getTimeSpent(), 1);
        break;
    }
    case RobotState::GO_STORAGES: {
        action_completed = goStorageZone(getTimeSpent());
        break;
    }
    case RobotState::GO_RIGHT_LINE:{
        goRightLimit(getTimeSpent());
        break;
    }
    case RobotState::GO_STORAGE_MADURO: {
        action_completed = goStorage(getTimeSpent(), 2); // FETCH_STOREHOUSE_MADURO
        break;
    }
    case RobotState::GO_STORAGE_SOBREMADURO: {
        action_completed = goStorage(getTimeSpent(), 3);  // FETCH_STOREHOUSE_SOBREMADURO
        break;
    }
    case RobotState::DROP_BEANS: {
        action_completed = dropBeans(getTimeSpent(), droped_SOBREMADURO+2);
        break;
    } */
    default:
        break;
    }
}