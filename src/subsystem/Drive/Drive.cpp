/*
 * @file drive.cpp
 * @date 2025-03-20
 * @author Juan Pablo Gutiérrez
 *
 * @brief Drive class, which controls the robot's drive system.
 */

#include "Drive.hpp"
#include "../../robot/robot_instances.h"

Drive::Drive() : front_left_(Pins::kUpperMotors[0], Pins::kUpperMotors[1], Pins::kPwmPin[0], true, Pins::kEncoders[0], Pins::kEncoders[1], DriveConstants::kWheelDiameter),
                 front_right_(Pins::kUpperMotors[2], Pins::kUpperMotors[3], Pins::kPwmPin[1], false, Pins::kEncoders[2], Pins::kEncoders[3], DriveConstants::kWheelDiameter),
                 back_left_(Pins::kLowerMotors[0], Pins::kLowerMotors[1], Pins::kPwmPin[2], true, Pins::kEncoders[4], Pins::kEncoders[5], DriveConstants::kWheelDiameter),
                 back_right_(Pins::kLowerMotors[2], Pins::kLowerMotors[3], Pins::kPwmPin[3], false, Pins::kEncoders[6], Pins::kEncoders[7], DriveConstants::kWheelDiameter),
                 bno_(),
                 robot_constants_(0.3, 0.3),
                 drive_controller_(),
                 heading_controller_()
{
}

void Drive::begin()
{
    bno_.begin();
    front_left_.begin();
    front_right_.begin();
    back_left_.begin();
    back_right_.begin();
}

void Drive::update()
{
    bno_.update();

    ChassisSpeed drive_speed;
    switch (drive_state_)
    {
    case DriveState::HEADING_LOCK:
    {
        drive_speed = drive_controller_.update(Rotation2D::fromDegrees(bno_.getYaw()), false);
        drive_speed.setOmega(heading_controller_.update(Rotation2D::fromDegrees(bno_.getYaw())));
        desired_chassis_speed_ = drive_speed;
    }
    break;
    case DriveState::FIELD_ORIENTED:
    {
        desired_chassis_speed_ = drive_controller_.update(Rotation2D::fromDegrees(bno_.getYaw()), true);
    }
    break;
    case DriveState::ROBOT_ORIENTED:
    {
        drive_speed = drive_controller_.update(Rotation2D::fromDegrees(bno_.getYaw()), false);
        desired_chassis_speed_ = drive_speed;
    }
    break;
    }

    move(desired_chassis_speed_);
}

void Drive::setState(int state)
{
    drive_state_ = static_cast<DriveState>(state);
}

void Drive::acceptInput(float vx, float vy, float omega)
{
    drive_controller_.acceptInput(vx, vy, omega);
}

void Drive::acceptHeadingInput(Rotation2D heading)
{
    heading_controller_.setDesiredHeading(heading);
}

void Drive::move(ChassisSpeed chassis_speed)
{
    int front_left_speed = chassis_speed.getVx() + chassis_speed.getVy() + chassis_speed.getOmega();
    int front_right_speed = -chassis_speed.getVx() + chassis_speed.getVy() - chassis_speed.getOmega();
    int back_left_speed = -chassis_speed.getVx() + chassis_speed.getVy() + chassis_speed.getOmega();
    int back_right_speed = chassis_speed.getVx() + chassis_speed.getVy() - chassis_speed.getOmega();

    front_left_.move(front_left_speed);
    front_right_.move(front_right_speed);
    back_left_.move(back_left_speed);
    back_right_.move(back_right_speed);
}

/* Basic Movement Functions */

void Drive::moveForward(int speed)
{
    front_left_.move(speed);
    front_right_.move(speed);
    back_left_.move(speed);
    back_right_.move(speed);
}

void Drive::moveBackward(int speed)
{
    front_left_.move(-speed);
    front_right_.move(-speed);
    back_left_.move(-speed);
    back_right_.move(-speed);
}

void Drive::moveLeft(int speed)
{
    front_left_.move(-speed);
    front_right_.move(speed);
    back_left_.move(speed);
    back_right_.move(-speed);
}

void Drive::moveRight(int speed)
{
    front_left_.move(speed);
    front_right_.move(-speed);
    back_left_.move(-speed);
    back_right_.move(speed);
}

void Drive::brake()
{
    front_left_.stop();
    front_right_.stop();
    back_left_.stop();
    back_right_.stop();
}

void Drive::hardBrake()
{
    front_left_.brakeStop();
    front_right_.brakeStop();
    back_left_.brakeStop();
    back_right_.brakeStop();
}

void Drive::motorTest()
{
    front_left_.move(100);
    delay(3000);
    front_left_.move(0);
    delay(3000);

    front_left_.move(-100);
    delay(3000);
    front_left_.move(0);
    delay(3000);

    front_right_.move(100);
    delay(3000);
    front_right_.move(0);
    delay(3000);

    front_right_.move(-100);
    delay(3000);
    front_right_.move(0);
    delay(3000);

    back_left_.move(100);
    delay(3000);
    back_left_.move(0);
    delay(3000);

    back_left_.move(-100);
    delay(3000);
    back_left_.move(0);
    delay(3000);

    back_right_.move(100);
    delay(3000);
    back_right_.move(0);
    delay(3000);

    back_right_.move(-100);
    delay(3000);
    back_right_.move(0);
    delay(3000);
}

/* Movement with cm */
// @brief: This will be used when the robot is align with the endline,
// the actual error of distance is around 1-2 cm, need a tunning
void Drive::moveForwardCm(float distance_cm, int speed)
{
    resetEncoders();
    float target_distance = distance_cm / 100.0f;
    
    while (getAverageDistanceTraveled() < target_distance)
    {
        moveForward(speed);
        delay(10); // Small delay for stability
    }
    brake();
}

void Drive::moveBackwardCm(float distance_cm, int speed)
{
    resetEncoders();
    float target_distance = distance_cm / 100.0f;
    
    while (getAverageDistanceTraveled() < target_distance)
    {
        moveBackward(speed);
        delay(10);
    }
    brake();
}

void Drive::moveLeftCm(float distance_cm, int speed)
{
    resetEncoders();
    float target_distance = distance_cm / 100.0f;
    
    while (getAverageDistanceTraveled() < target_distance)
    {
        moveLeft(speed);
        delay(10);
    }
    brake();
}

void Drive::moveRightCm(float distance_cm, int speed)
{
    resetEncoders();
    float target_distance = distance_cm / 100.0f;
    
    while (getAverageDistanceTraveled() < target_distance)
    {
        moveRight(speed);
        delay(10);
    }
    brake();
}

void Drive::resetEncoders()
{
    front_left_.getEncoderCount();
    front_right_.getEncoderCount();
    back_left_.getEncoderCount();
    back_right_.getEncoderCount();
}

float Drive::getAverageDistanceTraveled()
{
    float fl_distance = abs(front_left_.getPositionMeters());
    float fr_distance = abs(front_right_.getPositionMeters());
    float bl_distance = abs(back_left_.getPositionMeters());
    float br_distance = abs(back_right_.getPositionMeters());
    
    return (fl_distance + fr_distance + bl_distance + br_distance) / 4.0f;
}

void Drive::followFrontLine(int movement) {
    // movement = 0 -> left
    // movement = 1 -> right

    std::vector<int> sensors = line_sensor_.readSensors();
    
    bool front_left_detects = sensors[0];   
    bool front_right_detects = sensors[1];  
    bool back_left_detect = sensors[2];
    
    line_error_ = calculateLineError(sensors);
    float pid_output = calculateLinePID();
    
    float start_time = 0;
    int base_lateral_speed = 70;
    int correction_speed = (int)pid_output;
    
    if(start_time == 0){
        start_time = millis();
    }

    if (front_left_detects && front_right_detects) {
        if (movement == 0) { 
            if(millis() - start_time > 1500 ){
                moveLeft(70);
            }else{
                acceptInput(0,0,0);
            }
        } else { 
            if(millis() - start_time > 1500 ){
                moveRight(70);
            }else{
                acceptInput(0,0,0);
            }
        }
        return;
    }else if (front_left_detects && !front_right_detects) {
        front_left_.move(-base_lateral_speed + correction_speed);   
        front_right_.move(base_lateral_speed + correction_speed);     
        back_left_.move(base_lateral_speed + correction_speed);     
        back_right_.move(-base_lateral_speed + correction_speed);   
    }
    else if (!front_left_detects && front_right_detects) {
        front_left_.move(-base_lateral_speed - correction_speed);  
        front_right_.move(base_lateral_speed - correction_speed);  
        back_left_.move(base_lateral_speed - correction_speed);      
        back_right_.move(-base_lateral_speed - correction_speed);  
    }else{
        drive_.moveForward(50);
    }
}

void Drive::avoidFrontLine(){
    std::vector<int> sensors = line_sensor_.readSensors();
    
    bool front_left_detects = sensors[0];   
    bool front_right_detects = sensors[1];  
    
    if (front_left_detects || front_right_detects) {
        moveBackward(60);
    }else{
        moveLeft(60);
    }
}

void Drive::keepObstacleDistance(int movement){
    // movement = 0 -> left
    // movement = 1 -> right

    auto value = distance_sensor_.getArrayDistance();
    float front_left = value[0];
    float front_right = value[1];
    float min_distance = min(front_left, front_right);

    const float SAFE_DISTANCE = 30.0f;
    const float TOLERANCE = 5.0f;

    if(distance_sensor_.isObstacle())
    {
        float distance_error = min_distance - SAFE_DISTANCE;
        
        if(min_distance < SAFE_DISTANCE - TOLERANCE) {
            moveBackward(30);
            if(movement == 0){
                moveLeft(40);
            }else{
                moveRight(40);
            }
        }
        else if(min_distance > SAFE_DISTANCE + TOLERANCE) {
            moveForward(30);
            if(movement == 0){
                moveLeft(40);
            }else{
                moveRight(40);
            }
        }
        else {
            if(movement == 0){
                moveLeft(50);
            }else{
                moveRight(50);
            }
        }
    }
}

/*
void Drive::followFrontLine(int movement){
    std::vector<int> sensors = line_sensor_.readSensors();
    
    bool front_left_detects = sensors[0];   // FL sensor
    bool front_right_detects = sensors[1];  // FR sensor
    
    line_error_ = calculateLineError(sensors);
    
    float pid_output = calculateLinePID();
    
    int base_lateral_speed = 50;
    
    int correction_speed = (int)pid_output;
    
    if (front_left_detects && front_right_detects) {
        moveLeft(base_lateral_speed);
    }
    else if (front_left_detects && !front_right_detects) {
        front_left_.move(-base_lateral_speed + correction_speed);   
        front_right_.move(base_lateral_speed + correction_speed);     
        back_left_.move(base_lateral_speed + correction_speed);     
        back_right_.move(-base_lateral_speed + correction_speed);   
    }
    else if (!front_left_detects && front_right_detects) {
        front_left_.move(-base_lateral_speed - correction_speed);  
        front_right_.move(base_lateral_speed - correction_speed);  
        back_left_.move(base_lateral_speed - correction_speed);      
        back_right_.move(-base_lateral_speed - correction_speed);  
    }
}
*/

void Drive::setLinePIDConstants(float kp, float ki, float kd) {
    line_kp_ = kp;
    line_ki_ = ki;
    line_kd_ = kd;
}

float Drive::calculateLineError(const std::vector<int>& sensors) {
    // sensors[0] = FL (Front Left)
    // sensors[1] = FR (Front Right)
    // sensors[2] = BL (Back Left) 
    // sensors[3] = BR (Back Right)
    
    bool front_left_detects = sensors[0]; 
    bool front_right_detects = sensors[1]; 
    
    float error = 0.0f;
    
    if (front_left_detects && front_right_detects) {
        error = 0.0f;
    }
    else if (front_left_detects && !front_right_detects) {
        error = 1.0f;  
    }
    else if (!front_left_detects && front_right_detects) {
        error = -1.0f;   
    }
    else {
        error = line_last_error_;
    }
    
    return error;
}

float Drive::calculateLinePID() {
    unsigned long current_time = millis();
    float dt = (current_time - line_last_time_) / 1000.0f;
    
    if (line_last_time_ == 0) {
        dt = 0.02f; 
    }
    
    // Componente proporcional
    float P = line_kp_ * line_error_;
    
    // Componente integral con límites anti-windup más restrictivos
    line_integral_ += line_error_ * dt;
    line_integral_ = constrain(line_integral_, -50, 50);
    float I = line_ki_ * line_integral_;
    
    // Componente derivativo
    float D = 0.0f;
    if (dt > 0) {
        D = line_kd_ * (line_error_ - line_last_error_) / dt;
    }
    
    // Salida total del PID con normalización
    float output = P + I + D;
    output = constrain(output, -1.0f, 1.0f); // Normalizar salida
    
    line_last_error_ = line_error_;
    line_last_time_ = current_time;
    
    return output;
}
