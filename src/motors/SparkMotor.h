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
        int lowerDeadBand;
        int upperDeadBand;
        HardwarePWM* pwmModule;
        bool inverted = false;

        public:
        SparkMotor() = default;

        SparkMotor(const int PWMPin, HardwarePWM* pwmModule);

        void set(double speed) override;

        void stopMotor() override; 

        void invert(bool inverted) override;

        void setLowerDeadBand(int deadBand) override;
        void setUpperDeadBand(int deadBand) override;

        int getLowerDeadBand() override;
        int getUpperDeadBand() override;
    };
}

#endif