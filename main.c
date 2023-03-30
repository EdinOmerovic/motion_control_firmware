// Implement high level trajectory tracking algorithm using the low-level drivers/libraries

#include <stdio.h>

#include "encoder.h"
#include "motor.h"
#include "trajectory.h"
#include "control.h"
#include "lp_filter.h"

#define TIME_STEP 1000 // 1000[ns]
#define KP_val 1
#define KI_val 1
#define KD_val 1

#define AN 1
#define G_val 1

int getCalculationTime(void)
{
    // Return time spent on:
    // * Obtaining measurements
    // * Data processing
    // * Algorithm processing
    // * Actuator control

    return 0;
}

void sleep(int time)
{

}


void ISR_pb1(void){
    // endstop 1 reached

    // Recalibrate encoder, null position
}

void ISR_pb2(void){
    // endstop 2 reached
    
}

int main()
{
    // **** Instantation ****

    // Encoder
    Encoder enc;
    EncoderConf enc_conf;
    enc_conf.scalingParameteres = 0;
    enc_conf.absoluteDimentsions = 0;
    enc_conf.startingValue = 0;

    // Controler
    ControlerConf controler_conf;
    controler_conf.KP = KP_val;
    controler_conf.KI = KI_val;
    controler_conf.KD = KD_val;
    controler_conf.G = G_val;
    controler_conf.An = AN;

    // Motor
    Motor motor;
    MotorConf motor_conf;

    // Disturbance observer

    LowPassFilter lp_filter;
    init_filter(&lp_filter, G_val, 0);

    // // **** INITIALIZATION *****
    encoder_init(&enc);
    trajectory_init(FIXED);

    // KP, KI, KD
    control_init(&controler_conf);

    motor_init(&motor, &motor_conf);

    // // **** CONFIGURATION ****
    enc.configure(&enc, &enc_conf);

    int prev_vel = 0;
    int prev_pos = 0;
    while (1)
    {
        // Get the current trajectory:
        int q_ref = getTrajectory();

        // FIXME: dodaj da se osnovu funkcije može odrediti 2 izvod trajektorije
        int q2_ref = getTrajectory2od();

        // Get current position from encoder
        int q_act = enc.getValue(&enc);
        // Calculate velocity
        int vel = (q_act - prev_pos)*TIME_STEP;

        // Calculate error
        int error = q_ref - q_act;

        int q2_des = q2_ref + pd_control(error);

        // Disturbance observer
        int tau_dis = disturbance_observer1(&lp_filter, vel);

        // tau = tau_des + tau_dis; tau_des = an*q2_des;
        int tau = AN*q2_des + tau_dis;
        int q_act_new = enc.getValue(&enc);
        
        // Update disturbance observer
        disturbance_observer2(tau, q_act_new);


        motor.setPosition(&motor, tau);

        // Wait for the descrete timestep. Substract the time spent calculating
        prev_pos = enc.getValue(&enc);
        sleep(TIME_STEP - getCalculationTime());
    }

    return 0;
}