#include "StateMachine.h"

StateMachine::StateMachine()
{
}

void StateMachine::begin()
{
  currentState = STATES::START;
  state_start_time = 0;
}

void StateMachine::update()
{
  startStateTime();

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
  case STATES::PICKUP:
    handlePickupState();
    break;
  case STATES::RETURN:
    handleReturnState();
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
  action_start_time = 0;
  action_stage = 0;
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
  Serial.println("START STATE");

  if (action_stage == 1)
  {
    if (millis() - action_start_time > 350)
    {
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();
      action_stage = 0;
      setState(STATES::AVOID_OBSTACLE_LEFT);
    }
    return;
  }

  drive_.acceptInput(0, 90, 0);

  if (millis() - state_start_time > 3000)
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    state_start_time = 0;
    setState(STATES::GO_STRAIGHT);
    return;
  }

  if (millis() - state_start_time > 1500)
  {
    auto [obstacle, valid] = distance_sensor_.isObstacle();
    if (obstacle && valid)
    {
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();
      action_stage = 1;
      action_start_time = millis();
      return;
    }
  }
}

void StateMachine::handleAvoidObstacleLeftState()
{
  Serial.println("AVOID OBSTACLE LEFT STATE");

  // if the distance is greater than the max target distance, it means we've reached the edge of the pool with one sensor, so we should keep moving until both sensors dont see the pool
  auto [leftDistance, leftValid] = distance_sensor_.getDistance(0);
  if (leftDistance > DistanceSensorConstants::kObstacleDistance && leftValid)
  {
    Serial.println("LEFT SENSOR CLEAR");
    drive_.acceptInput(-130, 0, 0);
  }
  else
  {
    maintainDistance(DistanceSensorConstants::kPoolTargetDistance, -130);
  }

  if (line_sensor_.isLeftLine())
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    setState(STATES::AVOID_OBSTACLE_RIGHT);
    return;
  }

  auto [isObstacle, isValid] = distance_sensor_.isObstacle();
  if (!isObstacle && isValid)
  {
    delay(400);
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    setState(STATES::GO_STRAIGHT);
    return;
  }
}

void StateMachine::handleAvoidObstacleRightState()
{
  Serial.println("AVOID OBSTACLE RIGHT STATE");

  auto [rightDistance, rightValid] = distance_sensor_.getDistance(1);
  if (rightDistance > DistanceSensorConstants::kObstacleDistance && rightValid)
  {
    Serial.println("RIGHT SENSOR CLEAR");
    drive_.acceptInput(130, 0, 0);
  }
  else
  {
    Serial.println("RIGHT SENSOR BLOCKED");
    maintainDistance(DistanceSensorConstants::kPoolTargetDistance, 130);
  }

  if (line_sensor_.isRightLine())
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    setState(STATES::AVOID_OBSTACLE_LEFT);
    return;
  }

  auto [isObstacle, isValid] = distance_sensor_.isObstacle();
  if (!isObstacle && isValid)
  {
    delay(400);
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    setState(STATES::GO_STRAIGHT);
    return;
  }
}

void StateMachine::handleGoStraightState()
{
  Serial.println("GO_STRAIGHT STATE");

  if (action_stage == 1)
  {
    if (millis() - action_start_time > 250)
    {
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();
      action_stage = 0;
      setState(STATES::ENDLINE);
    }
    return;
  }

  if (line_sensor_.isFrontLine())
  {
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    drive_.acceptInput(0, -100, 0);

    action_stage = 1;
    action_start_time = millis();
    return;
  }
  else
  {
    drive_.acceptInput(0, 80, 0);
  }
}

void StateMachine::handleEndlineState()
{
  Serial.println("ENDLINE STATE");
  Serial.println("Action Stage: " + String(action_stage));

  if (action_stage == 0)
  {
    drive_.acceptInput(-145, 0, 0);

    if (line_sensor_.isCenterLine())
    {
      action_stage = 1;
      action_start_time = millis();
      drive_.acceptInput(0, -70, 0);
    }
  }

  else if (action_stage == 1)
  {
    if (millis() - action_start_time > 700)
    {
      action_stage = 0;
    }
  }
  else if (action_stage == 2)
  {
    if (line_sensor_.isFrontRightLine() || line_sensor_.isCenterLine())
    {
      Serial.println("FRONT RIGHT OR CENTER LINE DETECTED");
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();
      setState(STATES::PICKUP);
    }
    else
    {
      Serial.println("FRONT RIGHT LINE NOT DETECTED");
      drive_.acceptInput(0, 90, 0);
    }
  }

  if (action_stage != 2)
  {

    if (line_sensor_.isBackLeftLine())
    {
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();
      action_stage = 2;
    }
  }
}

void StateMachine::handlePickupState()
{
  static bool visionReady = false;
  static int lastTop = -1;
  static int lastBottom = -1;

  Serial.println("PICKUP STATE");
  // string cmd = com_.getCommand();

  // if (!visionReady)
  // {
  //   if (cmd == "XAVIER RUNNING VISION")
  //   {
  //     visionReady = true;
  //     Serial.println("[INFO] Xavier vision online — starting movement");
  //   }
  //   else
  //   {
  //     drive_.acceptInput(0, 0, 0);
  //     return;
  //   }
  // }

  followLineHybrid(145, 0.02f);

  // auto values = com_.getMatrix();
  // int top = values[0];
  // int bottom = values[1];
  // Serial.println("TOP: " + String(top) + "BOTTOM: " + String(bottom));

  // if (top != -1 && top != lastTop)
  // {
  //   if (top == 2 || top == 1)
  //   {
  //     upperIntake_.setState(1);
  //   }
  //   else if (top == 0)
  //   {
  //     upperIntake_.setState(0);
  //   }
  //   lastTop = top;
  // }

  // if (bottom != -1 && bottom != lastBottom)
  // {
  //   if (bottom == 2 || bottom == 1)
  //   {
  //     lowerIntake_.setState(1);
  //   }
  //   else if (bottom == 0)
  //   {
  //     lowerIntake_.setState(0);
  //   }
  //   lastBottom = bottom;
  // }

  if (line_sensor_.isBackRightLine())
  {
    drive_.acceptInput(0, 0, 0);
    setState(STATES::RETURN);
    return;
  }
}

// ================ RETURNING STATES ===================
// Keep in mind that when returning, left and right are swapped
// ======================================================

void StateMachine::handleReturnState()
{
  Serial.println("RETURN STATE");

  if (action_stage == 1)
  {
    if (millis() - action_start_time > 350)
    {
      drive_.acceptInput(0, 0, 0);
      drive_.acceptHeadingInput(Rotation2D::fromDegrees(180));
      elevator_.setState(2);
      action_stage = 2;
    }
    return;
  }
  else if (action_stage == 2)
  {
    if (drive_.isAtHeadingTarget())
    {
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();
      action_stage = 3;
      action_start_time = millis();
      distance_sensor_.clearReadings();
    }
    return;
  }
  else if (action_stage == 3)
  {
    if (millis() - action_start_time > 450)
    {
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();
      action_stage = 0;
      setState(STATES::AVOID_OBSTACLE_RIGHT_RETURN);
    }

    return;
  }
  else if (action_stage == 0)
  {
    drive_.acceptInput(-90, -70, 0);
    action_stage = 1;
    action_start_time = millis();
  }
}

// Keep in mind that when returning, left and right are swapped
// this function is somewhat hardcoded, im sorry
void StateMachine::handleAvoidObstacleRightReturnState()
{
  Serial.println("AVOID OBSTACLE RIGHT STATE");

  auto [rightDistance, rightValid] = distance_sensor_.getDistance(1);
  if (rightDistance > 50 && rightValid)
  {
    Serial.println("MANUAL");
    drive_.acceptInput(120, 0, 0);
  }
  else
  {
    Serial.println("AUTOMATIC");
    maintainDistance(DistanceSensorConstants::kPoolTargetDistance, 120);
  }

  // we use only the left sensor, we're moving to the right, so when the left sensor is clear, we are clear of the obstacle
  auto [leftDistance, leftValid] = distance_sensor_.getDistance(0);
  if (leftDistance > 50)
  {
    delay(700);
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    drive_.acceptInput(0, -50, 0);
    delay(250);
    drive_.acceptInput(0, 0, 0);
    drive_.hardBrake();
    setState(STATES::GO_BEGINNING);
    return;
  }
}

void StateMachine::handleGoBeginningState()
{
  Serial.println("GO_BEGINNING STATE");

  if (action_stage == 0)
  {
    drive_.acceptInput(0, 100, 0);

    if (line_sensor_.isLeftLine())
    {
      action_stage = 1;
      action_start_time = millis();
      drive_.acceptInput(80, 0, 0);
    }
  }
  else if (action_stage == 1)
  {
    if (millis() - action_start_time > 700)
    {
      action_stage = 0;
    }
  }
  else if (action_stage == 2)
  {
    if (millis() - action_start_time > 250)
    {
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();
      action_stage = 0;
      setState(STATES::STOP);
    }
    return;
  }

  if (action_stage != 2)
  {
    if (line_sensor_.isCenterLine())
    {
      drive_.acceptInput(0, 0, 0);
      drive_.hardBrake();
      drive_.acceptInput(0, -100, 0);

      action_stage = 2;
      action_start_time = millis();
      Serial.println("SHUTDOWN");
      return;
    }
  }
}

void StateMachine::handleStopState()
{
  Serial.println("STOP STATE");

  drive_.acceptInput(0, 0, 0);
  drive_.hardBrake();
}