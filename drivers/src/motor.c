#include "motor.h"



void _setPosition(Motor *motor, int setpoint)
{
    // motor->cfg;
    // Analog write to the analogout.


}

void motor_init(Motor *motor, MotorConf *motor_confg)
{
    motor->cfg = motor_confg;
    motor->setPosition = _setPosition; 
}