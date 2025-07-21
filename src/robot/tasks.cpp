/**
 * @file tasks.cpp
 * @brief File for the tasks of the robot
 * @author Juan Pablo Gutiérrz
 * @date 2025-07-16
 */
#include "tasks.h"

void initStart()
{
    drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));
    drive_.acceptInput(0, 0, 0);
    drive_.setState(0);
}

void exitStart()
{
    drive_.acceptInput(0,200,0);
}

void avoidPool()
{
    float distance = distance_sensor_->getCurrentDistance();
    
    if(distance < 10){
        drive_.acceptInput(200,0,0);
    }else{
        drive_.acceptInput(0,200,0);
    }
}

void goToTrees(){
    if(com_->getCommand() == "Tree")
    {
        // TODO: Add the actions to perform when the command is received
    }
}

void setupTasks()
{
    xTaskCreate(globalUpdateTask, "Global Update Task", 10000, NULL, 1, &globalUpdateTaskHandle);
}

void globalUpdateTask(void *pvParameters)
{
    while (true)
    {
        drive_.update();
        // gripper_.update();
        // elevator_.update();
        // lower_sorter_.update();
        // upper_sorter_.update();
        // camera_.update();
        // line_sensor_.update();

        vTaskDelay(pdMS_TO_TICKS(SystemConstants::kUpdateInterval));
    }
}