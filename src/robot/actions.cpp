/**
 * @file actions.cpp
 * @brief File for the actions of the robot
 * @author Juan Pablo Gutiérrz
 * @date 2025-05-06
 */
#include "actions.hpp"

bool init()
{
    drive_.setState(0);
    gripper_.setState(0);
    elevator_.setState(0);
    lower_sorter_.setState(1);
    upper_sorter_.setState(1);
    return true;
}

bool globalUpdate()
{
    drive_.update();
    gripper_.update();
    elevator_.update();
    lower_sorter_.update();
    upper_sorter_.update();
    camera_.update();
    line_sensor_.update();

    return true;
}

bool centerWithObject(double elapsed_time) // Center in X offset
{
    float offsetX = camera_.getOffset_X();
    // float offsetY = camera_.getOffset_Y();
    if (offsetX > abs(0.5))
    {
        double outputX = centerPID_.update(offsetX, VisionConstants::kCenterOffsetX);
        // double outputY = centerPID_.update(offsetY, VisionConstants::kCenterOffsetY);
        drive_.acceptInput(-outputX, 0, 0);
        return false;
    }
    drive_.acceptInput(0, 0, 0);
    return true;
}

bool alignWithObject(double elapsed_time, float desired_distance = 10.0)
{ // Acomodar robot to a certain distance //! incomplete method
    float distance = camera_.getObjectDistance();
    if (distance > desired_distance)
    {
        drive_.acceptInput(0, 100, 0);
        return false;
    }
    drive_.acceptInput(0, 0, 0);
    return true;
}

bool pickBean(double elapsed_time, int level)
{
    static int state = 0;
    static double state_start_time = 0;

    bool beanType = true; // false = MADURO, true = SOBREMADURO

    if (state_start_time == 0)
    {
        state_start_time = elapsed_time;
    }

    switch (state)
    {
    case 0: // Set gripper and elevator
        gripper_.setState(1);
        state = 1;
        state_start_time = elapsed_time;
        return false;
    case 1: // Center and pick
            /* if (camera_.getBeanType() != BeanConstants::NONE) {
                BeanConstants::BeanType type = camera_.getBeanType();
                beanType = (type == BeanConstants::SOBREMADURO);  // false = MADURO, true = SOBREMADURO
                if (centerWithObject(elapsed_time)) {
                    state = 2;
                    state_start_time = elapsed_time;
                }
            } else {
            } */
        state = 2;
        state_start_time = elapsed_time;
        return false;

    case 2:
        if (elapsed_time - state_start_time < 750)
        {
            // drive_.acceptInput(0,100,0);
        }
        else
        {
            drive_.acceptInput(0, 0, 0);
            gripper_.setState(0);
            state = 3;
            state_start_time = elapsed_time;
        }
        return false;

    case 3:
        if (elapsed_time - state_start_time < 1000)
        {
            drive_.acceptInput(0,-100,0);
            upper_sorter_.setState(1);
        }
        else
        {
            drive_.acceptInput(0, 0, 0);
            state = 4;
            state_start_time = elapsed_time;
        }
        return false;

    case 4:
        if (elapsed_time - state_start_time < 1000)
        {
            gripper_.setState(1);
        }
        else
        {
            upper_sorter_.setState(beanType * 2);
            return true;
        }
        return false;
    default:
        state = 0;
        state_start_time = 0;
        return false;
    }
}

bool initStart()
{
    drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));
    drive_.acceptInput(0, 0, 0);
    drive_.setState(0);
    gripper_.setState(0);
    elevator_.setState(0);
    lower_sorter_.setState(1);
    upper_sorter_.setState(1);
    return true;
}

/**
 * @brief Avanzar por 2 segundos, definir la posicion de inicio
 */
bool exitStart(double elapsed_time)
{
    static int state = 0;
    static double state_start_time = 0;

    if (state_start_time == 0)
    {
        state_start_time = elapsed_time;
    }

    switch (state)
    {
    case 0: // Set heading and move
        drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));
        if (elapsed_time - state_start_time < 3250)
        {
            drive_.acceptInput(0, 100, 0);
            return false;
        }
        state = 1;
        return false;

    case 1: // Stop
        drive_.acceptInput(0, 0, 0);
        state = 0;
        state_start_time = 0;
        return true;

    default:
        state = 0;
        state_start_time = 0;
        return false;
    }
}

bool goTreeZone(double elapsed_time)
{ //! incomplete function, obstacle detection is missing
    drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));
    drive_.setState(0);

    static int state = 0;
    static double state_start_time = 0;

    if (state_start_time == 0)
    {
        state_start_time = elapsed_time;
    }

    switch (state)
    {
    case 0: // Advance
        if (elapsed_time - state_start_time < 5000)
        {
            drive_.acceptInput(0, 100, 0);
            return false;
        }
    case 1: // Stop
        drive_.acceptInput(0, 0, 0);
        return true;
    default:
        state = 0;
        state_start_time = 0;
        return false;
    }
}

bool goLeftLimit(double elapsed_time)
{
    drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));
    drive_.setState(0);

    static int state = 0;
    static double state_start_time = 0;

    if (state_start_time == 0)
    {
        state_start_time = elapsed_time;
    }

    switch (state)
    {
    case 0: // Search
        if (!line_sensor_.leftDetected())
        {
            drive_.acceptInput(-100, 0, 0);
            return false;
        }
        else
        {
            Serial.println("Left line detected!");
            state = 1;
        }
        return false;

    case 1: // Stop
        drive_.acceptInput(0, 0, 0);
        state = 0;
        state_start_time = 0;
        return true;

    default:
        state = 0;
        state_start_time = 0;
        return false;
    }
}

bool goRightLimit(double elapsed_time)
{
    drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));
    drive_.setState(0);

    static int state = 0;
    static double state_start_time = 0;

    if (state_start_time == 0)
    {
        state_start_time = elapsed_time;
    }

    switch (state)
    {
    case 0: // Search
        if (!line_sensor_.rightDetected())
        {
            drive_.acceptInput(100, 0, 0);
            return false;
        }
        state = 1;
        return false;

    case 1: // Stop
        drive_.acceptInput(0, 0, 0);
        state = 0;
        state_start_time = 0;
        return true;

    default:
        state = 0;
        state_start_time = 0;
        return false;
    }
}
/**
 * @brief Buscar arboles
 */
bool searchForTrees(double elapsed_time, bool direction)
{
    static int state = 0;
    static double state_start_time = 0;

    if (state_start_time == 0)
    {
        state_start_time = elapsed_time;
    }

    camera_.setState(1); // FETCH_TREES

    switch (state)
    {
    case 0: // Search
        if (elapsed_time - state_start_time < 2000 && !camera_.objectPresent())
        {
            drive_.acceptInput(direction ? 100 : -100, 0, 0);
            return false;
        }
        state = 1;
        return false;

    case 1: // Stop
        drive_.acceptInput(0, 0, 0);
        state = 0;
        state_start_time = 0;
        return true;

    default:
        state = 0;
        state_start_time = 0;
        return false;
    }
}

bool avoidPool(double elapsed_time)
{
    static int state = 0;
    static int state_start_time = 0;
    static int recovery_time = 0;

    switch (state)
    {
    case 0:
    {
        // Step 1: Stop + slight backoff
        if (!camera_.objectPresent())
            return true;
        drive_.acceptInput(0, -100, 0); // back up a bit
        if (elapsed_time > 500)
        {
            drive_.acceptInput(0, 0, 0),
                state++;
            state_start_time = elapsed_time;
        }
        break;
    }

    case 1:
    {
        // Initial
        // Response: Go left
        drive_.acceptInput(-100, 0, 0); // strafe left

        if (line_sensor_.leftDetected())
        {
            drive_.acceptInput(0, 0, 0);
            state = 2;
            state_start_time = elapsed_time;
            recovery_time = elapsed_time;
        }
        // Time should correlate to movement across one slot
        else if (elapsed_time - state_start_time > 1000)
        {
            // Check if pool
            if (camera_.objectPresent())
            {
                drive_.acceptInput(0, 0, 0);
                state = 3;
                state_start_time = elapsed_time;
            }
            else
            {
                // Coast is clear :)
                return true;
            }
        }

        break;
    }

    case 2:
    {
        // Left -> found line
        // Response: Go back -> go right
        drive_.acceptInput(100, 0, 0); // right

        if (elapsed_time - state_start_time > 1000 + recovery_time)
        {
            // Check if pool
            if (camera_.objectPresent())
            {
                drive_.acceptInput(0, 0, 0);
                state_start_time = elapsed_time;
                state = 4;
            }
            else
            {
                // Coast is clear :)
                return true;
            }
        }

        break;
    }

    case 3:
    {
        // Left -> found pool
        // Response: Go left
        drive_.acceptInput(-100, 0, 0); // strafe left

        if (line_sensor_.leftDetected())
        {
            drive_.acceptInput(0, 0, 0);
            state = 5;
            state_start_time = elapsed_time;
            recovery_time = elapsed_time;
        }
        // Time should correlate to movement across one slot
        else if (elapsed_time - state_start_time > 1000)
        {
            // Check if pool
            if (camera_.objectPresent())
            {
                drive_.acceptInput(0, 0, 0);
                state = 3;
                state_start_time = elapsed_time;
            }
            else
            {
                // Coast is clear :)
                return true;
            }
        }
        break;
    }

    case 4:
    {
        // left -> found line -> returned -> right -> pool
        // Response: Go right
        drive_.acceptInput(100, 0, 0); // strafe right

        // Time should correlate to movement across one slot
        if (elapsed_time - state_start_time > 1000)
        {
            // Check if pool (not strictly necessary)
            if (camera_.objectPresent())
            {
                drive_.acceptInput(0, 0, 0);
                state = 4;
                state_start_time = elapsed_time;
            }
            else
            {
                // Coast is clear :)
                return true;
            }
        }
        break;
    }
    case 5:
    {
        // left -> found pool -> found line
        // Response: Go back, go right, go right
        if (elapsed_time - state_start_time > recovery_time + 2000)
        {
            // Check if pool (not strictly necessary)
            if (camera_.objectPresent())
            {
                drive_.acceptInput(0, 0, 0);
                state = 4;
                state_start_time = elapsed_time;
            }
            else
            {
                // Coast is clear :)
                return true;
            }
        }
        break;
    }
    }
    return false;
}

bool goStorageZone(double elapsed_time)
{
    drive_.acceptHeadingInput(Rotation2D::fromDegrees(180));
    drive_.setState(0);

    static int state = 0;
    static double state_start_time = 0;

    if (state_start_time == 0)
    {
        state_start_time = elapsed_time;
    }

    switch (state)
    {
    case 0: // Advance
        if (elapsed_time - state_start_time < 5000)
        {
            drive_.acceptInput(0, 100, 0);
            return false;
        }
    case 1: // Stop
        drive_.acceptInput(0, 0, 0);
        return true;
    default:
        state = 0;
        state_start_time = 0;
        return false;
    }
}

bool goStorage(double elapsed_time, int container_type)
{
    static int state = 0;
    static double state_start_time = 0;

    if (state_start_time == 0)
    {
        state_start_time = elapsed_time;
    }

    camera_.setState(container_type); // 2 = FETCH_STOREHOUSE_MADURO | 3 = FETCH_STOREHOUSE_SOBREMADURO

    switch (state)
    {
    case 0: // Search
        if (elapsed_time - state_start_time < 2000 && !camera_.objectPresent())
        {
            drive_.acceptInput(-100, 0, 0);
            return false;
        }
        state = 1;
        return false;

    case 1: // Stop
        drive_.acceptInput(0, 0, 0);
        state = 0;
        state_start_time = 0;
        return true;

    default:
        state = 0;
        state_start_time = 0;
        return false;
    }
}

bool dropBeans(double elapsed_time, int container_type)
{
    static int state = 0;
    static double state_start_time = 0;

    if (state_start_time == 0)
    {
        state_start_time = elapsed_time;
    }

    switch (state)
    {
    case 0: // Center with storage
        camera_.setState(container_type);
        if (centerWithObject(elapsed_time))
        {
            state = 1;
            state_start_time = elapsed_time;
        }
        return false;

    case 1: // Turn 180 degrees
        drive_.setState(0);
        drive_.acceptHeadingInput(Rotation2D::fromDegrees(180));
        if (elapsed_time - state_start_time > 1000)
        {
            state = 2;
            state_start_time = elapsed_time;
        }
        return false;

    case 2: // Move backwards
        if (elapsed_time - state_start_time < 2000)
        {
            drive_.acceptInput(0, -100, 0);
        }
        else
        {
            state = 3;
            state_start_time = elapsed_time;
        }
        return false;

    case 3: // Drop beans
        drive_.acceptInput(0, 0, 0);
        lower_sorter_.setState(container_type);
        if (elapsed_time - state_start_time > 1000)
        {
            state = 4;
            state_start_time = elapsed_time;
        }
        return false;

    case 4: // Move forward and turn back
        drive_.setState(0);
        drive_.acceptInput(0, 100, 0);
        drive_.acceptHeadingInput(Rotation2D::fromDegrees(0));
        if (elapsed_time - state_start_time > 2000)
        {
            state = 5;
            state_start_time = 0;
            return true;
        }
        return false;

    default:
        state = 0;
        state_start_time = 0;
        return false;
    }
}


/* 
void moveToLow(double time_elapsed)
{
    if (time_elapsed < 1000)
    {
        Serial.print("SET_SPEED:");
        Serial.println(50);
    }
    else
    {
        Serial.print("SET_SPEED:");
        Serial.println(0);
    }
}

void moveToMid(double time_elapsed)
{
    if (time_elapsed < 3000)
    {
        Serial.print("SET_SPEED:");
        Serial.println(100);
        gripper_.setState(1);
    }
    else if (time_elapsed < 4000)
    {
        Serial.print("SET_SPEED:");
        Serial.println(0);
    }
    else
    {
        Serial.print("SET_SPEED:");
        Serial.println(0);
        gripper_.setState(0);
    }
} */