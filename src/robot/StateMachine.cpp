#include "StateMachine.hpp"
#include "robot/robot_instances.h"

StateMachine::StateMachine(SoftwareSerial &bluetoothRef)
    : bluetooth(bluetoothRef), state_start_time(0), after_obstacle_offset(200), currentState(STATES::START)
{
}

void StateMachine::begin()
{
  currentState = STATES::GO_STRAIGHT;
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
  // case STATES::RETURN:
  //   handleReturnState();
  //   break;
  // case STATES::AVOID_OBSTACLE_LEFT_RETURN:
  //   handleAvoidObstacleLeftReturnState();
  //   break;
  // case STATES::AVOID_OBSTACLE_RIGHT_RETURN:
  //   handleAvoidObstacleRightReturnState();
  //   break;
  // case STATES::GO_BEGINNING:
  //   handleGoBeginningState();
  //   break; 
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

  maintainDistance(DistanceSensorConstants::kPoolTargetDistance, -75);

  if (line_sensor_.isLeftLine())
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    currentState = STATES::AVOID_OBSTACLE_RIGHT;
  }

  if (!distance_sensor_.isObstacle())
  {
    if(millis() - state_start_time > 3000){
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();
      currentState = STATES::GO_STRAIGHT;
    }
  }

}

void StateMachine::handleAvoidObstacleRightState()
{
  Serial.println("AVOID OBSTACLE RIGHT STATE");
  bluetooth.println("AVOID OBSTACLE RIGHT STATE");

  maintainDistance(DistanceSensorConstants::kPoolTargetDistance, 75);

  if (line_sensor_.isRightLine())
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    currentState = STATES::AVOID_OBSTACLE_LEFT;
  }

  if (!distance_sensor_.isObstacle())
  {
    if(millis() - state_start_time > 3000){
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();
      currentState = STATES::GO_STRAIGHT;
    }
  }
}

void StateMachine::handleGoStraightState()
{
  Serial.println("GO_STRAIGHT STATE");
  bluetooth.println("GO_STRAIGHT STATE");

  if (state_start_time == 0)
  {
    state_start_time = millis();
  }

  if (line_sensor_.isFrontLine())
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    if (millis() - state_start_time > 2500)
    {
      state_start_time = 0;
      currentState = STATES::ENDLINE;
    }
  }
  else
  {
    drive_.acceptInput(0, 70, 0);
  }
}

void StateMachine::handleEndlineState()
{
  Serial.println("ENDLINE STATE");
  bluetooth.println("ENDLINE STATE");

  followLine(-70);
  if (line_sensor_.isLeftLine()) // Add the && for the frontLine to assure the robot is well positioned
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();

    if (state_start_time == 0)
    {
      state_start_time = millis();
    }

    if (millis() - state_start_time > 2000)
    {
      state_start_time = 0;
      currentState = STATES::RIGHTMOST;
    }
  }
}

void StateMachine::handleRightmostState()
{
  Serial.println("RIGHTMOST STATE");
  bluetooth.println("RIGHTMOST STATE");

  // followLine(70);

  // if (line_sensor_.isRightLine()) // same as above
  // {
  //   drive_.acceptInput(0, 0, 0);
  //   currentState = STATES::RETURN;
  // }

  currentState = STATES::STOP;
}

void StateMachine::handleReturnState()
{
  Serial.println("RETURN STATE");
  bluetooth.println("RETURN STATE");
  drive_.acceptInput(0, 0, 0);

  // if (distance_sensor_.isObstacle())
  // {
  //   currentState = STATES::AVOID_OBSTACLE_RIGHT_RETURN;
  // }
  // else if (!distance_sensor_.obstacleInThePath())
  // {
  //   currentState = STATES::GO_BEGINNING;
  // }
  // else
  // {
  //   drive_.acceptInput(0, 70, 180);
  // }
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

  if (!distance_sensor_.isObstacle() && millis() - state_start_time > 12000)
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

  if (!distance_sensor_.isObstacle() && millis() - state_start_time > 12000)
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