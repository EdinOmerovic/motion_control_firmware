#include "motor.h"
#include "dac_hal.h"
#define SCALER 100

int map(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void _setTorque(Motor *motor, int torque)
{
    // Na osnovu to
    // motor->cfg;
    // Analog write to the analogout.
    int voltage = map(torque, -10, 10, 0, 33);
    // Push to DAC
    DAC_PTR[DAC_NUM]->DACVALS.all = voltage / SCALER;
}

void motor_init(Motor *motor, MotorConf *motor_confg)
{
    motor->cfg = motor_confg;
    motor->setTorque = _setTorque;
}
