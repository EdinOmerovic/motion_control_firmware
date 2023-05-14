#include "main.h"
#include "motor.h"
#include "dac_hal.h"
#include "helpers.h"

#define RANGE_IN_MIN MIN_MOTOR_TAU// min value of torque
#define RANGE_IN_MAX MAX_MOTOR_TAU// max value of torque
#define RANGE_OUT_MIN 0
#define RANGE_OUT_MAX 4095



void _setTorque(Motor *motor, signed long torque)
{
    Uint32 voltage = map(torque, RANGE_IN_MIN, RANGE_IN_MAX, RANGE_OUT_MIN, RANGE_OUT_MAX);
    // Push to DAC
    DAC_PTR[DAC_NUM]->DACVALS.all = voltage / motor->cfg->scaler;
}

void motor_init(Motor *motor, MotorConf *motor_confg)
{
    motor->cfg = motor_confg;
    motor->setTorque = _setTorque;
}
