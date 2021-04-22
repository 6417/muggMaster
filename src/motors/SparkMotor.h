#ifndef SPARKMOTOR_H 
#define SPARKMOTOR_H

#include <Arduino.h>

namespace motor
{
    class SparkMotor
    {
        protected:
        int PWMPin;
        bool isInverted;
        int deadBand;
        
        public:
        SparkMotor() = default;

        SparkMotor(int PWMPin);

        void set(double speed);

        void stopMotor();

        void setMotorDirection(bool inverted);

        void setDeadBand(int deadBand);

        int getDeadBand();
    };
}

#endif