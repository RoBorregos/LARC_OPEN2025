#include "StateMachine.hpp"
#include "robot/robot_instances.h"

StateMachine::StateMachine(Monitor &monitorRef)
    : monitor(monitorRef), state_start_time(0), after_obstacle_offset(200), currentState(STATES::START)
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
  monitor_.println("START STATE");

  drive_.acceptInput(0, 90, 0);

  if (state_start_time == 0)
  {
    state_start_time = millis();
  }

  // if (millis() - state_start_time > 3000)
  // {
  //   drive_.acceptInput(0, 0, 0);
  //   drive_.hardBrake();
  //   state_start_time = 0;
  //   currentState = STATES::GO_STRAIGHT;
  //   return;
  // }

  auto [obstacle, valid] = distance_sensor_.isObstacle();
  if (obstacle && valid)
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    drive_.acceptInput(0, -50, 0);
    delay(250);
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    state_start_time = 0;
    currentState = STATES::AVOID_OBSTACLE_LEFT;
    return;
  }

}

void StateMachine::handleAvoidObstacleLeftState()
{
  monitor_.println("AVOID OBSTACLE LEFT STATE");

  // // if the distance is greater than the max target distance, it means we've reached the edge of the pool with one sensor, so we should keep moving until both sensors dont see the pool
  // if (distance_sensor_.getDistance(0) > DistanceSensorConstants::kMaxTargetDistance || distance_sensor_.getDistance(1) > DistanceSensorConstants::kMaxTargetDistance)
  // {
  //   drive_.acceptInput(-75, 0, 0);
  // }
  // else
  // {
  // }
  maintainDistance(DistanceSensorConstants::kPoolTargetDistance, 100);

  // if (line_sensor_.isLeftLine())
  // {
  //   drive_.acceptInput(0, 0, 0);
  //   drive_.hardBrake();
  //   currentState = STATES::AVOID_OBSTACLE_RIGHT;
  //   return;
  // }

  // if (!distance_sensor_.isObstacle())
  // {
  //   delay(500);
  //   drive_.acceptInput(0, 0, 0);
  //   drive_.hardBrake();
  //   currentState = STATES::GO_STRAIGHT;
  //   return;
  // }
}

void StateMachine::handleAvoidObstacleRightState()
{
  Serial.println("AVOID OBSTACLE RIGHT STATE");
  monitor_.println("AVOID OBSTACLE RIGHT STATE");

  auto [distance, valid] = distance_sensor_.getDistance(1);
  if (distance > DistanceSensorConstants::kMaxTargetDistance || distance > DistanceSensorConstants::kMaxTargetDistance && valid)
  {
    drive_.acceptInput(75, 0, 0);
  }
  else
  {
    maintainDistance(DistanceSensorConstants::kPoolTargetDistance, 110);
  }

  if (line_sensor_.isFrontRightLine())
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    currentState = STATES::AVOID_OBSTACLE_LEFT;
    return;
  }

  auto [isObstacle, rightValid] = distance_sensor_.isObstacle();
  if (!isObstacle && rightValid)
  {
    delay(500);
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    currentState = STATES::GO_STRAIGHT;
    return;
  }
}

void StateMachine::handleGoStraightState()
{
  monitor_.println("GO_STRAIGHT STATE");

  if (line_sensor_.isFrontLine())
  {
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
  currentState = STATES::STOP;
  // Serial.println("ENDLINE STATE");
  // monitor_.println("ENDLINE STATE");

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

  followLine(70);

  if (line_sensor_.isRightLine())
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    currentState = STATES::RETURN;
  }
}

void StateMachine::handleReturnState()
{
  monitor_.println("RETURN STATE");
  drive_.acceptHeadingInput(Rotation2D::fromDegrees(180));
}

void StateMachine::handleAvoidObstacleLeftReturnState()
{
  monitor_.println("AVOID OBSTACLE LEFT RETURN STATE");
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

  auto [obstacle, valid] = distance_sensor_.isObstacle();
  if (!obstacle || millis() - state_start_time > 12000)
  {
    state_start_time = 0;
    drive_.acceptInput(0, 0, 180);
    drive_.hardBrake();
    currentState = STATES::GO_BEGINNING;
  }
}

void StateMachine::handleAvoidObstacleRightReturnState()
{
  monitor_.println("AVOID OBSTACLE RIGHT RETURN STATE");
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

  auto [obstacle, valid] = distance_sensor_.isObstacle();
  if (!obstacle || millis() - state_start_time > 12000)
  {
    state_start_time = 0;
    drive_.acceptInput(0, 0, 180);
    drive_.hardBrake();
    currentState = STATES::GO_BEGINNING;
  }
}

void StateMachine::handleGoBeginningState()
{
  monitor_.println("GO BEGINNING STATE");
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
  drive_.acceptInput(0, 0, 0);
  drive_.hardBrake();
}