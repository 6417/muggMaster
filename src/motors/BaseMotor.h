#ifndef BASEMOTOR_H
#define BASEMOTOR_H

#include <Arduino.h>
#include "utilities\utilities.h"

namespace motor
{
    class BaseMotor
    {
        protected:

        int deadBand;

        public:
        virtual void set(double speed) = 0;

        virtual void stopMotor() = 0;

        virtual void invert(bool inverted) = 0;

        virtual void setDeadBand(int deadBand)
        {
            this->deadBand = deadBand;
        }

        virtual int getDeadBand()
        {
            return deadBand;
        }
    };
}
#endif