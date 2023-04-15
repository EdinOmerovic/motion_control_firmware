#include "motor.h"
#include "dac_hal.h"

#define RANGE_IN_MIN 10 // min value of torque
#define RANGE_IN_MAX 10 // max value of torque
#define RANGE_OUT_MIN 0
#define RANGE_OUT_MAX 4095

Uint32 map(signed long x, signed long in_min, signed long in_max, Uint32 out_min, Uint32 out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void _setTorque(Motor *motor, signed long torque)
{
    Uint32 voltage = map(torque, -RANGE_IN_MIN, RANGE_IN_MAX, RANGE_OUT_MIN, RANGE_OUT_MAX);
    // Push to DAC
    DAC_PTR[DAC_NUM]->DACVALS.all = voltage / motor->cfg->scaler;
}

void motor_init(Motor *motor, MotorConf *motor_confg)
{
    motor->cfg = motor_confg;
    motor->setTorque = _setTorque;
}
