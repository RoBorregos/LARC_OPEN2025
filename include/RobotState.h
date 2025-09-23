/*
 * @file RobotState.h
 * @date 02/04/2025
 * @author  Dana E. Torres Estrada
 *
 * @brief Defines the RobotState enumeration representing different states of the robot.
 */

#ifndef ROBOTSTATE_H
#define ROBOTSTATE_H

enum class RobotState
{
    START, // Represents the start of the robot
    AVOID_OBSTACLE_LEFT,
    AVOID_OBSTACLE_RIGHT,
    GO_STRAIGHT,
    ENDLINE,
    RIGHTMOST,
    RETURN
};

#endif // ROBOTSTATE_H
