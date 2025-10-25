#include "StateMachine.hpp"
#include "robot/robot_instances.h"

StateMachine::StateMachine(Monitor &monitorRef)
    : monitor(monitorRef), state_start_time(0), currentState(STATES::START)
{
}

void StateMachine::begin()
{
  currentState = STATES::RETURN;
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
    Serial.println("Error: Unknown state.");
    drive_.acceptInput(0, 0, 0);
    break;
  }
}

void StateMachine::setState(STATES newState)
{
  state_start_time = 0;
  currentState = newState;
}

void StateMachine::startStateTime()
{
  if (state_start_time == 0)
  {
    state_start_time = millis();
  }
}

void StateMachine::handleStartState()
{
  monitor_.println("START STATE");
  startStateTime();

  drive_.acceptInput(0, 90, 0);

  if (millis() - state_start_time > 3000)
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    state_start_time = 0;
    setState(STATES::GO_STRAIGHT);
    return;
  }

  auto [obstacle, valid] = distance_sensor_.isObstacle();
  if (obstacle && valid)
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    drive_.acceptInput(0, -50, 0);
    delay(250);
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    setState(STATES::STOP);
    return;
  }
}

void StateMachine::handleAvoidObstacleLeftState()
{
  monitor_.println("AVOID OBSTACLE LEFT STATE");
  startStateTime();

  // if the distance is greater than the max target distance, it means we've reached the edge of the pool with one sensor, so we should keep moving until both sensors dont see the pool
  auto [distance, valid] = distance_sensor_.getDistance(0);
  if (distance > DistanceSensorConstants::kObstacleDistance && valid)
  {
    Serial.println("LEFT SENSOR CLEAR");
    drive_.acceptInput(80, 0, 0);
  }
  else
  {
    maintainDistance(DistanceSensorConstants::kPoolTargetDistance, -80);
  }

  if (line_sensor_.isLeftLine())
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    setState(STATES::AVOID_OBSTACLE_RIGHT);
    return;
  }

  auto [isObstacle, leftValid] = distance_sensor_.isObstacle();
  if (!isObstacle && leftValid)
  {
    delay(700);
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    drive_.acceptInput(0, -50, 0);
    delay(250);
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    setState(STATES::GO_STRAIGHT);
    return;
  }
}

void StateMachine::handleAvoidObstacleRightState()
{
  monitor_.println("AVOID OBSTACLE RIGHT STATE");
  startStateTime();

  auto [distance, valid] = distance_sensor_.getDistance(1);
  if (distance > DistanceSensorConstants::kObstacleDistance && valid)
  {
    Serial.println("RIGHT SENSOR CLEAR");
    drive_.acceptInput(80, 0, 0);
  }
  else
  {
    Serial.println("RIGHT SENSOR BLOCKED");
    maintainDistance(DistanceSensorConstants::kPoolTargetDistance, 80);
  }

  if (line_sensor_.isRightLine())
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    setState(STATES::AVOID_OBSTACLE_LEFT);
    return;
  }

  auto [isObstacle, rightValid] = distance_sensor_.isObstacle();
  if (!isObstacle && rightValid)
  {
    delay(700);
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    drive_.acceptInput(0, -50, 0);
    delay(250);
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    setState(STATES::GO_STRAIGHT);
    return;
  }
}

void StateMachine::handleGoStraightState()
{
  monitor_.println("GO_STRAIGHT STATE");
  startStateTime();

  if (line_sensor_.isFrontLine())
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    setState(STATES::STOP);
  }
  else
  {
    drive_.acceptInput(0, 80, 0);
  }
}

void StateMachine::handleEndlineState()
{
  monitor_.println("ENDLINE STATE");
  startStateTime();

  setState(STATES::STOP);
  // Serial.println("ENDLINE STATE");

  // followLine(-70);

  // if (line_sensor_.isLeftLine())
  // {
  //   drive_.acceptInput(0, 0, 0);
  //   drive_.hardBrake();
  //   currentState = STATES::RIGHTMOST;
  // }
}

void StateMachine::handleRightmostState()
{
  monitor_.println("RIGHTMOST STATE");
  startStateTime();

  followLine(70);

  if (line_sensor_.isRightLine())
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    setState(STATES::RETURN);
  }
}

void StateMachine::handleReturnState()
{
  monitor_.println("RETURN STATE");
  startStateTime();

  drive_.acceptInput(0, -40, 0);
  drive_.acceptHeadingInput(Rotation2D::fromDegrees(180));
  drive_.acceptInput(0, -40, 180);
  drive_.hardBrake();
  setState(STATES::STOP);
}

void StateMachine::handleAvoidObstacleLeftReturnState()
{
  monitor_.println("AVOID OBSTACLE LEFT RETURN STATE");
  startStateTime();

  drive_.acceptInput(0, 0, 0);

  if (line_sensor_.isLeftLine())
  {
    setState(STATES::AVOID_OBSTACLE_RIGHT_RETURN);
  }
  else
  {
    drive_.acceptInput(-80, 0, 180);
  }

  auto [obstacle, valid] = distance_sensor_.isObstacle();
  if (!obstacle || millis() - state_start_time > 12000)
  {
    drive_.acceptInput(0, 0, 180);
    drive_.hardBrake();
    setState(STATES::GO_BEGINNING);
  }
}

void StateMachine::handleAvoidObstacleRightReturnState()
{
  monitor_.println("AVOID OBSTACLE RIGHT RETURN STATE");
  startStateTime();

  drive_.acceptInput(0, 0, 0);

  if (line_sensor_.isRightLine())
  {
    setState(STATES::AVOID_OBSTACLE_LEFT_RETURN);
  }
  else
  {
    drive_.acceptInput(80, 0, 180);
  }

  auto [obstacle, valid] = distance_sensor_.isObstacle();
  if (!obstacle || millis() - state_start_time > 12000)
  {
    drive_.acceptInput(0, 0, 180);
    drive_.hardBrake();
    setState(STATES::GO_BEGINNING);
  }
}

void StateMachine::handleGoBeginningState()
{
  monitor_.println("GO BEGINNING STATE");
  startStateTime();
  
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
  monitor_.println("STOP STATE");
  startStateTime();

  drive_.acceptInput(0, 0, 0);
  drive_.hardBrake();
}