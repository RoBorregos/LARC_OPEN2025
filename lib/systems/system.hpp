/**
 * @file system.hpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez & Dana E. Torres Estrada
 *  
 * @brief Header file for the System class, which is the base class for all systems in the robot.
 */

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

class System {
public:
    System();
    virtual void update();
    virtual void setState(int state);
};

#endif //SYSTEM_HPP