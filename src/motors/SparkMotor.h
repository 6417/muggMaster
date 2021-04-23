#ifndef SPARKMOTOR_H 
#define SPARKMOTOR_H

#include <Arduino.h>
#include "utilities/utilities.h"

// pwm highfreq pins = 22, 23

namespace motor
{
    class SparkMotor
    {
        protected:
        int PWMPin;
        bool isInverted;
        int deadBand;
        HardwarePWM* pwmModule;
        bool inverted = false;

        public:
        SparkMotor() = default;

        SparkMotor(int PWMPin, HardwarePWM* pwmModule);

        void set(double speed);

        void stopMotor();

        void invert(bool inverted);

        void setDeadBand(int deadBand);

        int getDeadBand();
    };
}

#endif
