#include"SpeedControllerGroup.h"

SpeedControllerGroup::SpeedControllerGroup(motor::L298N_Motor* motors, size_t size)
{
    this->motors = motors;
    this->size = size;
}
void SpeedControllerGroup::set(double speed)
{
    for(size_t i = 0; i < this->size; ++i)
    {
        motors[i].set(speed);
    }
}

void SpeedControllerGroup::setMotorDirection(boolean inverted)
{
    for(size_t i = 0; i < this->size; ++i)
    {
        motors[i].setMotorDirection(inverted);
    }
}
void SpeedControllerGroup::stopMotors()
{
    for(size_t i = 0; i < this->size; ++i)
    {
        motors[i].stopMotor();
    }
}