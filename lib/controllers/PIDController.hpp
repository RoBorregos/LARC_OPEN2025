#ifndef PID_CONTROLLER_HPP
#define PID_CONTROLLER_HPP

#include <PID_v1.h>
#include <Arduino.h>

class PIDController {
public:
    /**
     * @brief Default constructor
     */
    PIDController();
    
    /**
     * @brief Construct a new PID Controller
     * 
     * @param kp Proportional gain
     * @param ki Integral gain
     * @param kd Derivative gain
     * @param outputMin Minimum output value
     * @param outputMax Maximum output value
     */
    PIDController(float kp, float ki, float kd, float outputMin = -1.0f, float outputMax = 1.0f);
    
    /**
     * @brief Destroy the PID Controller
     */
    ~PIDController();
    
    /**
     * @brief Update the PID controller with a new measurement
     * 
     * @param measurement Current measurement
     * @param setpoint Desired setpoint
     * @return float The calculated output
     */
    float update(float measurement, float setpoint);
    
    /**
     * @brief Reset the PID controller
     */
    void reset();
    
    /**
     * @brief Set the PID gains
     * 
     * @param kp Proportional gain
     * @param ki Integral gain
     * @param kd Derivative gain
     */
    void setGains(float kp, float ki, float kd);
    
    /**
     * @brief Set the output limits
     * 
     * @param min Minimum output value
     * @param max Maximum output value
     */
    void setOutputLimits(float min, float max);
    
    /**
     * @brief Enable or disable the PID controller
     * 
     * @param enabled Whether the controller should be enabled
     */
    void setEnabled(bool enabled);
    
    /**
     * @brief Check if the PID controller is enabled
     * 
     * @return true if enabled
     * @return false if disabled
     */
    bool isEnabled() const;
    
    /**
     * @brief Get the current error
     * 
     * @return float The current error
     */
    float getError() const;
    
    /**
     * @brief Get the current output
     * 
     * @return float The current output
     */
    float getOutput() const;
    
    /**
     * @brief Set the angle wrapping
     * 
     * @param enabled Whether angle wrapping should be enabled
     */
    void setAngleWrapping(bool enabled);
    
    /**
     * @brief Check if angle wrapping is enabled
     * 
     * @return true if enabled
     * @return false if disabled
     */
    bool isAngleWrappingEnabled() const;

private:
    double lastMeasurement_;
    double lastSetpoint_;
    double output_;
    bool enabled_;
    bool angleWrapping_;
    PID* pid_;
};

#endif // PID_CONTROLLER_HPP 