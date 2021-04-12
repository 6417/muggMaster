#include "TankDrive.h"

using namespace tankdrive;

TankDrive::TankDrive(motor::L298N_Motor* leftMotor, motor::L298N_Motor* rightMotor)
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

