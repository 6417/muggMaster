#include "SparkMotor.h"

using namespace motor;
using namespace utilities;

SparkMotor::SparkMotor(const int PWMPin, HardwarePWM* pwmModule)
{
    this->PWMPin = PWMPin;
    this->pwmModule = pwmModule;   
    this->pwmModule->addPin(PWMPin);
}

void SparkMotor::set(double speed)
{
    double output;

    if(inverted)
    {
        output = static_cast<int>(mapDouble(speed, 1, -1, lowerDeadBand, upperDeadBand));
    }
    else if(!inverted)
    {
        output = static_cast<int>(mapDouble(speed, -1, 1, lowerDeadBand, upperDeadBand));
    }
    output = abs(output);
    output = constrain(output, 8000, 16000);

    pwmModule->writePin(PWMPin, output);
    // Serial.println(PWMPin);
    Serial.println(output);
}

void SparkMotor::stopMotor()
{
}

void SparkMotor::invert(bool inverted)
{
    this->inverted = inverted;
}


void SparkMotor::setLowerDeadBand(int deadBand)
{
    this->lowerDeadBand = deadBand;
}
int SparkMotor::getLowerDeadBand()
{
    return deadBand;
}

void SparkMotor::setUpperDeadBand(int deadBand)
{
    this->upperDeadBand = deadBand;
}
int SparkMotor::getUpperDeadBand()
{
    return upperDeadBand;
}