#include "StateMachine.hpp"
#include "robot/robot_instances.h"

StateMachine::StateMachine(SoftwareSerial &bluetoothRef)
    : bluetooth(bluetoothRef), state_start_time(0), after_obstacle_offset(200), currentState(STATES::START)
{
}

void StateMachine::begin()
{
  currentState = STATES::START;
  state_start_time = 0;
}

void StateMachine::update()
{
  switch (currentState)
  {
  case STATES::START:
    handleStartState();
    break;
  case STATES::AVOID_OBSTACLE_LEFT:
    handleAvoidObstacleLeftState();
    break;
  case STATES::AVOID_OBSTACLE_RIGHT:
    handleAvoidObstacleRightState();
    break;
  case STATES::GO_STRAIGHT:
    handleGoStraightState();
    break;
  case STATES::ENDLINE:
    handleEndlineState();
    break;
  case STATES::RIGHTMOST:
    handleRightmostState();
    break;
  case STATES::RETURN:
    handleReturnState();
    break;
  case STATES::AVOID_OBSTACLE_LEFT_RETURN:
    handleAvoidObstacleLeftReturnState();
    break;
  case STATES::AVOID_OBSTACLE_RIGHT_RETURN:
    handleAvoidObstacleRightReturnState();
    break;
  case STATES::GO_BEGINNING:
    handleGoBeginningState();
    break;
  case STATES::STOP:
    handleStopState();
    break;
  default:
    Serial.println("Error: Estado desconocido.");
    drive_.acceptInput(0, 0, 0);
    break;
  }
}

STATES StateMachine::getCurrentState() const
{
  return currentState;
}

void StateMachine::setState(STATES newState)
{
  currentState = newState;
}

void StateMachine::handleStartState()
{
  Serial.println("START STATE");
  bluetooth.println("START STATE");

  drive_.acceptInput(0, 70, 0);

  if (state_start_time == 0)
  {
    state_start_time = millis();
  }

  if (millis() - state_start_time > 3000)
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    state_start_time = 0;
    currentState = STATES::GO_STRAIGHT;
  }

  if (distance_sensor_.isObstacle())
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    state_start_time = 0;
    currentState = STATES::AVOID_OBSTACLE_LEFT;
  }
}

void StateMachine::handleAvoidObstacleLeftState()
{
  Serial.println("AVOID OBSTACLE LEFT STATE");
  bluetooth.println("AVOID OBSTACLE LEFT STATE");

  // if the distance is greater than the max target distance, it means we've reached the edge of the pool with one sensor, so we should keep moving until both sensors dont see the pool
  if (distance_sensor_.getDistance(0) > DistanceSensorConstants::kMaxTargetDistance || distance_sensor_.getDistance(1) > DistanceSensorConstants::kMaxTargetDistance)
  {
    drive_.acceptInput(-75, 0, 0);
  }
  else
  {
    maintainDistance(DistanceSensorConstants::kPoolTargetDistance, -75);
  }

  if (line_sensor_.isLeftLine())
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    currentState = STATES::AVOID_OBSTACLE_RIGHT;
  }

  if (!distance_sensor_.isObstacle())
  {
    delay(500);
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    currentState = STATES::GO_STRAIGHT;
  }
}

void StateMachine::handleAvoidObstacleRightState()
{
  Serial.println("AVOID OBSTACLE RIGHT STATE");
  bluetooth.println("AVOID OBSTACLE RIGHT STATE");

  if (distance_sensor_.getDistance(0) > DistanceSensorConstants::kMaxTargetDistance || distance_sensor_.getDistance(1) > DistanceSensorConstants::kMaxTargetDistance)
  {
    drive_.acceptInput(75, 0, 0);
  }
  else
  {
    maintainDistance(DistanceSensorConstants::kPoolTargetDistance, 75);
  }

  if (line_sensor_.isFrontRightLine())
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    currentState = STATES::AVOID_OBSTACLE_LEFT;
  }

  if (!distance_sensor_.isObstacle())
  {
    delay(500);
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    currentState = STATES::GO_STRAIGHT;
  }
}

void StateMachine::handleGoStraightState()
{
  Serial.println("GO_STRAIGHT STATE");
  bluetooth.println("GO_STRAIGHT STATE");

  if (line_sensor_.isFrontLine())
  {
    drive_.acceptInput(0, 15, 0);
    delay(100); 
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    currentState = STATES::ENDLINE;
  }
  else
  {
    drive_.acceptInput(0, 40, 0);
  }
}

void StateMachine::handleEndlineState()
{
  Serial.println("ENDLINE STATE");
  bluetooth.println("ENDLINE STATE");
  
  followLine(-55);
  
  if(line_sensor_.isLeftLine())
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    currentState = STATES::RIGHTMOST;
  }
}

void StateMachine::handleRightmostState()
{
  Serial.println("RIGHTMOST STATE");
  bluetooth.println("RIGHTMOST STATE");

  followLine(55);

  if(line_sensor_.isRightLine())
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    currentState = STATES::RETURN;
  }
}

void StateMachine::handleReturnState()
{
  Serial.println("RETURN STATE");
  bluetooth.println("RETURN STATE");
  drive_.acceptHeadingInput(Rotation2D::fromDegrees(180));
}

void StateMachine::handleAvoidObstacleLeftReturnState()
{
  Serial.println("AVOID OBSTACLE LEFT RETURN STATE");
  bluetooth.println("AVOID OBSTACLE LEFT RETURN STATE");
  drive_.acceptInput(0, 0, 0);

  if (line_sensor_.isLeftLine())
  {
    state_start_time = 0;
    currentState = STATES::AVOID_OBSTACLE_RIGHT_RETURN;
  }
  else
  {
    drive_.acceptInput(-80, 0, 180);
  }

  if (state_start_time == 0)
  {
    state_start_time = millis();
  }

  if (!distance_sensor_.isObstacle() || millis() - state_start_time > 12000)
  {
    state_start_time = 0;
    drive_.acceptInput(0, 0, 180);
    drive_.hardBrake();
    currentState = STATES::GO_BEGINNING;
  }
}

void StateMachine::handleAvoidObstacleRightReturnState()
{
  Serial.println("AVOID OBSTACLE RIGHT RETURN STATE");
  bluetooth.println("AVOID OBSTACLE RIGHT RETURN STATE");
  drive_.acceptInput(0, 0, 0);

  if (line_sensor_.isRightLine())
  {
    state_start_time = 0;
    currentState = STATES::AVOID_OBSTACLE_LEFT_RETURN;
  }
  else
  {
    drive_.acceptInput(80, 0, 180);
  }

  if (state_start_time == 0)
  {
    state_start_time = millis();
  }

  if (!distance_sensor_.isObstacle() || millis() - state_start_time > 12000)
  {
    state_start_time = 0;
    drive_.acceptInput(0, 0, 180);
    drive_.hardBrake();
    currentState = STATES::GO_BEGINNING;
  }
}

void StateMachine::handleGoBeginningState()
{
  Serial.println("GO BEGINNING STATE");
  bluetooth.println("GO BEGINNING STATE");
  drive_.acceptInput(0, 0, 0);

  if (line_sensor_.isFrontLine())
  {
    drive_.acceptInput(0, 0, 0);
    Serial.println("FOUND THE BEGINNING");
  }
  else
  {
    drive_.acceptInput(0, 70, 180);
  }
}

void StateMachine::handleStopState()
{
  Serial.println("STOP STATE");
  bluetooth.println("STOP STATE");
  drive_.acceptInput(0, 0, 0);
  drive_.hardBrake();
}