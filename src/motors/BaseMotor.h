#ifndef BASEMOTOR_H
#define BASEMOTOR_H

#include <Arduino.h>
#include "utilities\utilities.h"

namespace motor
{
    class BaseMotor
    {
        protected:
        int lowerDeadBand;
        int upperDeadBand;

        public:
        virtual void set(double speed) = 0;

        virtual void stopMotor() = 0;

        virtual void invert(bool inverted) = 0;


        virtual void setLowerDeadBand(int deadBand)
        {
            this->lowerDeadBand = deadBand;
        }
        virtual void setUpperDeadBand(int deadBand)
        {
            this->upperDeadBand = deadBand;
        }

        virtual int getLowerDeadBand()
        {
            return lowerDeadBand;
        }
        virtual int getUpperDeadBand()
        {
            return upperDeadBand;
        }
    };
}
#endif