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
