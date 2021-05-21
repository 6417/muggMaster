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
        output = static_cast<int>(mapDouble(speed, 1, -1, 8000, 16000));
    }
    else if(!inverted)
    {
        output = static_cast<int>(mapDouble(speed, -1, 1, 8000, 16000));
    }
    output = abs(output);
    output = constrain(output, lowerDeadBand, upperDeadBand);

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
    this->lowerDeadBand = lowerDeadBand;
}
int SparkMotor::getLowerDeadBand()
{
    return lowerDeadBand;
}

void SparkMotor::setUpperDeadBand(int deadBand)
{
    this->upperDeadBand = deadBand;
}
int SparkMotor::getUpperDeadBand()
{
    return upperDeadBand;
}