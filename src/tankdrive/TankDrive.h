#ifndef TANKDRIVE_H
#define TANKDRIVE_H

#include "motors/L298N_Motor.h"
#include "motors/SpeedControllerGroup.h"
#include "motors/BaseMotor.h"
#include <Arduino.h>

namespace tankdrive
{
    class TankDrive
    {
        private:
        motor::BaseMotor* leftMotor;
        motor::BaseMotor* rightMotor;

        public:
        TankDrive(motor::BaseMotor* rightMotor, motor::BaseMotor* leftMotor);

        void drive(double x, double y);
    };
}

#endif 