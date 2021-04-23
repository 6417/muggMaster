#include "TankDrive.h"

using namespace tankdrive;

TankDrive::TankDrive(motor::BaseMotor* leftMotor, motor::BaseMotor* rightMotor)
{
    this->leftMotor = leftMotor;
    this->rightMotor = rightMotor;
}

void TankDrive::drive(double x, double y)
{
    double outputRight = x + y;
    double outputLeft = x - y;

    // Serial.printf("right: %f left: %f \n", outputRight, outputLeft);
    leftMotor->set(outputLeft);
    rightMotor->set(outputRight);
}

