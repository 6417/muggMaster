#ifndef SPARKMOTOR_H 
#define SPARKMOTOR_H

#include <Arduino.h>
#include "utilities/utilities.h"
#include "BaseMotor.h"

// pwm highfreq pins = 22, 23

namespace motor
{
    class SparkMotor : public BaseMotor
    {
        protected:
        int PWMPin;
        bool isInverted;
        int deadBand;
        HardwarePWM* pwmModule;
        bool inverted = false;

        public:
        SparkMotor() = default;

        SparkMotor(const int PWMPin, HardwarePWM* pwmModule);

        virtual void set(double speed) override;

        virtual void stopMotor() override; 

        virtual void invert(bool inverted) override;

        virtual void setDeadBand(int deadBand) override;

        virtual int getDeadBand() override;
    };
}

#endif
