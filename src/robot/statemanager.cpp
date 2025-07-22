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

    // case RobotState::EXIT_START: {
    //     drive_.acceptInput(0,200,0);
    //     break;
    // }
    // case RobotState::GO_TREES: {
    //     if(com_.getCommand() == "DETECTED TREEE"){
    //         drive_.acceptInput(0,200,0);
    //     }
    //     break;
    // }
    /*
    case RobotState::AVOID_POOL: { //TODO:
        action_completed = avoidPool(getTimeSpent());
        gripper_.setState(1);
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
    */
    // case RobotState::PICK_MID_LEVEL: {
    //     elevator_.setState(2);
    //     if(com_.getCommand() ==  "BEAN DETECTED"){
    //         gripper_.setState(1);
    //         delay(1000);
    //         gripper_.setState(1);
    //     }
    //     break;
    // }
    // case RobotState::PICK_LOW_LEVEL: {
    //     if(com_.getCommand() ==  "BEAN DETECTED"){
    //         gripper_.setState(1);
    //         delay(1000);
    //     }
    //     break;
    // }
    /*
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
    */
    // case RobotState::DROP_BEANS: {
    //     lower_sorter_.setState(0);
    //     break;
    // } 
    default:
        break;
    }
}