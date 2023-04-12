#include "motor.h"
#include "dac_hal.h"

#define RANGE_IN_MIN 10
#define RANGE_IN_MAX 10
#define RANGE_OUT_MIN 0
#define RANGE_OUT_MAX 33

int map(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void _setTorque(Motor *motor, int torque)
{
    // Na osnovu to
    // motor->cfg;
    // Analog write to the analogout.
    int voltage = map(torque, -RANGE_IN_MIN, RANGE_IN_MAX, RANGE_OUT_MIN, RANGE_OUT_MAX);
    // Push to DAC
    DAC_PTR[DAC_NUM]->DACVALS.all = voltage / motor->cfg->scaler;
}

void motor_init(Motor *motor, MotorConf *motor_confg)
{
    motor->cfg = motor_confg;
    motor->setTorque = _setTorque;
}
