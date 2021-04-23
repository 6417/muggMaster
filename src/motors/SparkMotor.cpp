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
    output = constrain(output, 8000, 16000);

    pwmModule->writePin(PWMPin, output);
    Serial.println(PWMPin);
}

void SparkMotor::stopMotor()
{
    
}

void SparkMotor::invert(bool inverted)
{
    this->inverted = inverted;
}

void SparkMotor::setDeadBand(int deadBand)
{
    this->deadBand = deadBand;
}

int SparkMotor::getDeadBand()
{
    return deadBand;
}