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
  PICKUP,
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

  // Used for general timing within states (i.e after 3 seconds from state start, do X)
  unsigned long state_start_time = 0;

  // Used for multi-stage actions within states (i.e start an action triggered by a sensor within a state, and then after some time do another action)
  unsigned long action_start_time = 0;
  int action_stage = 0;

  std::vector<int> values = {0, 0};
  unsigned long last_coffee_value_received = 0;
  static constexpr unsigned long recive_coffee_values_interval = 800;

  void handleStartState();
  void handleAvoidObstacleLeftState();
  void handleAvoidObstacleRightState();
  void handleGoStraightState();
  void handleEndlineState();
  void handlePickupState();
  void handleReturnState();
  void handleAvoidObstacleLeftReturnState();
  void handleAvoidObstacleRightReturnState();
  void handleGoBeginningState();
  void handleStopState();

  void startStateTime();
};

#endif // STATEMACHINE_HPP