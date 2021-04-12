#include "L298N_Motor.h"
#include "utilities/utilities.h"


motor::L298N_Motor::L298N_Motor(int enable, int inA, int inB)
{
    this->enable = enable;
    pinMode(enable, OUTPUT);

    this->inA = inA;
    pinMode(inA, OUTPUT);

    this->inB = inB;
    pinMode(inB, OUTPUT);
}

void motor::L298N_Motor::setMotorDirection(bool inverted)
{
    if (inverted)
    {
        digitalWrite(inA, HIGH);
        digitalWrite(inB, LOW);
    }
    else if (!inverted)
    {
        digitalWrite(inA, LOW);
        digitalWrite(inB, HIGH);
    }
}

void motor::L298N_Motor::set(double speed)
{
    double inputSpeed = abs(speed);
    int output = static_cast<int>(utilities::mapDouble(inputSpeed, 0, 1, deadBand, 200));
    // Serial.printf("output: %d speed: %f deadBand: %d \n", output, speed, output);SKJDFÖ
    output = abs(output);
    output = constrain(output, 0, 255);

    if(speed > 0){
        setMotorDirection(true);
    }    
    else if(speed < 0){
        setMotorDirection(false);
    }
    if(output < deadBand)
    {
        stopMotor();   
        return;
    }

    analogWrite(enable, output);
}

void motor::L298N_Motor::stopMotor(){
    digitalWrite(enable, LOW);
    digitalWrite(inA, LOW); 
    digitalWrite(inB, LOW); 
}

void motor::L298N_Motor::setDeadBand(int deadBand)
{
    this->deadBand = deadBand;
}

int motor::L298N_Motor::getDeadBand()
{
    return this->deadBand;
}