/**
 * High level library for motor and motor control.
 * 
 * 
 * The responsibilities of this library are:
 *  * Provide signals necessary for tourque control of the linear motor using the motor controller.
 * 
 * Linear motor: FAULHABER LM2070-080-11C
 * Motor controller: FAULHABER MCLM3006 
 * 
*/

#ifndef MOTOR_H
#define MOTOR_H

typedef struct
{
    /* data */
} MotorConf;


typedef struct Motor
{
    // Function 
    void (*setPosition)(struct Motor *, int);
} Motor;


void motor_init(Motor  *, MotorConf *);


#endif