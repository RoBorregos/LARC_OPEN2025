#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "StateUtils.h"

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
private:
  STATES currentState;
  unsigned long state_start_time;
  uint8_t after_obstacle_offset;
  SoftwareSerial &bluetooth;

public:
  StateMachine(SoftwareSerial &bluetoothRef);

  void begin();
  void update();

  STATES getCurrentState() const;
  void setState(STATES newState);

private:
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
};

#endif // STATEMACHINE_HPP