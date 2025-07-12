/*
 * @file RobotState.h
 * @date 02/04/2025
 * @author  Dana E. Torres Estrada
 *
 * @brief Defines the RobotState enumeration representing different states of the robot.
 */

#ifndef ROBOTSTATE_H
#define ROBOTSTATE_H

enum class RobotState {
    INIT,
    //CALIBRATE, //(si lo hacemos bien)
    EXIT_START,
    GO_TREES,
    AVOID_POOL,
    GO_LEFT_LINE,
    GO_RIGHT_LINE,
    SEARCH_TREES,
    PICK_MID_LEVEL,
    PICK_LOW_LEVEL,
    MOVE_NEXT_TREE,
    GO_STORAGES,
    GO_STORAGE_MADURO,
    GO_STORAGE_SOBREMADURO, //Robo no tendrá esto
    DROP_BEANS,
    IDLE,
};

#endif //ROBOTSTATE_H
