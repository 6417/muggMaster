#ifndef SPEEDCONTROLLERGROUP_H
#define SPEEDCONTROLLERGROUP_H

#include <Arduino.h>
#include "L298N_Motor.h"

class SpeedControllerGroup
{
protected:
    motor::L298N_Motor* motors;
    size_t size;

public:
    SpeedControllerGroup(motor::L298N_Motor* motors, size_t size);

    void set(double);

    void setMotorDirection(boolean inverted);

    void stopMotors();
};

#endif