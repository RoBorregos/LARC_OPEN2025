/**
 * @file bno.hpp
 * @date 22/04/2025
 * @author Juan Pablo Gutiérrez & Dana E. Torres Estrada
 *  
 * @brief Header file for the BNO class, which is the class for the BNO sensor.
 */

#ifndef BNO_HPP
#define BNO_HPP

#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>

class BNO
{
    public:
        BNO();
        bool begin();
        void update();
        float getRoll() const;
        float getPitch() const;
        float getYaw() const;

    private:
        Adafruit_BNO055 bno;
        sensors_event_t event;
        bool initialized;

        float wrapAngle(float angle) const;
};

#endif