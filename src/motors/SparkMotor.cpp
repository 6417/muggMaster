#include "SparkMotor.h"

using namespace motor;

SparkMotor::SparkMotor(int PWMPin)
{
    this->PWMPin = PWMPin;
    pinMode(PWMPin, OUTPUT);
}

void SparkMotor::set(double speed)
{

}

void SparkMotor::stopMotor()
{

}

void SparkMotor::setMotorDirection(bool inverted)
{

}

void SparkMotor::setDeadBand(int deadBand)
{
}

int SparkMotor::getDeadBand()
{
    return deadBand;
}