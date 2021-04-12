#ifndef TANKDRIVE_H
#define TANKDRIVE_H

#include "motors/L298N_Motor.h"
#include "motors/SpeedControllerGroup.h"
#include <Arduino.h>

namespace tankdrive
{
    class TankDrive
    {
        private:
        motor::L298N_Motor* rightMotor;
        motor::L298N_Motor* leftMotor;

        public:
        TankDrive(motor::L298N_Motor* leftMotor, motor::L298N_Motor* rightMotor);

        void drive(double x, double y);
    };
}

#endif 