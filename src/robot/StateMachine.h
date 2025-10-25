#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "StateUtils.h"
#include "robot/robot_instances.h"

enum class STATES
{
  START, // Represents the start of the robot
  AVOID_OBSTACLE_LEFT,
  AVOID_OBSTACLE_RIGHT,
  GO_STRAIGHT,
  ENDLINE,
  RIGHTMOST,
  AVOID_OBSTACLE_LEFT_RETURN,
  AVOID_OBSTACLE_RIGHT_RETURN,
  RETURN,
  GO_BEGINNING,
  STOP
};

class StateMachine
{
public:
  StateMachine();

  void begin();
  void update();

  void setState(STATES newState);

private:
  STATES currentState = STATES::START;
  unsigned long state_start_time = 0;

  void handleStartState();
  void handleAvoidObstacleLeftState();
  void handleAvoidObstacleRightState();
  void handleGoStraightState();
  void handleEndlineState();
  void handleRightmostState();
  void handleReturnState();
  void handleAvoidObstacleLeftReturnState();
  void handleAvoidObstacleRightReturnState();
  void handleGoBeginningState();
  void handleStopState();

  void startStateTime();
};

#endif // STATEMACHINE_HPP