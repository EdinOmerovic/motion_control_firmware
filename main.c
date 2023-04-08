// Implement high level trajectory tracking algorithm using the low-level drivers/libraries

// TODO: skontati u kojim jedinicama treba da bude pozicija.
// Fix the type of classses and objects. Some are singletone some are not

// MVP: za fiksnu poziciju robot treba da se uspješno vrati na željenu pozicijiu.
// Želimo da imamo jedinstvenu vrijednost referentne pozicije

// ************ UNITS ************
// For the sake of using non-floating point aritmetic, the units are:
// Length is in nanometers: 10^-9 [m]
// Time is i nanosecods: 10^-9 [s]
// Mass: TODO

#include <stdio.h>
// Contains includes and configuration parameters
#include "main.h"

// **** INSTATATION ****
// Encoder
Encoder enc;
EncoderConf enc_conf;

// Controler
ControlerConf controler_conf;

// Motor
Motor motor;
MotorConf motor_conf;

// LP filter
LowPassFilter lp_filter;

int main()
{
    // // **** CONFIGURATION SETUP *****
    // Encoder
    encoder_init(&enc);
    enc_conf.startingValue = 0;
    enc_conf.scalingFactor = 1000;
    enc_conf.absoluteDimentsions = 0;
    enc.configure(&enc, &enc_conf);

    // Controler
    controler_conf.An = AN;
    controler_conf.G = G_val;
    controler_conf.KP = KP_val;
    controler_conf.KI = KI_val;
    controler_conf.KD = KD_val;
    control_init(&controler_conf);

    // Desired trajectory:
    // *Hardcoded: suplied using lookup table
    // *Analog: obtained using analog read
    // *Fixed: fixed value of the trajectory
    trajectory_init(FIXED);

    filter_init(&lp_filter, G_val, 0);

    motor_init(&motor, &motor_conf);

    // **** Persistant loop variables ****
    int prev_pos = 0;
    while (1)
    {
        // Get the desired trajectory:
        int q_ref = getTrajectory();

        // FIXME: dodaj da se osnovu funkcije može odrediti 2 izvod trajektorije
        // Get the second derivative of the desired trajectory
        int q2_ref = getTrajectory2od(); // Za prvi MVP to je 0

        // Get current absolute position from encoder
        int q_act = enc.getValue(&enc);

        // Calculate position error
        int error = q_ref - q_act;

        // Calculate second derivative of q desired
        int q2_des = q2_ref + pd_control(error);

        // Disturbance observer
        // Calculate velocity:
        // v = delta X / delta T
        int vel = (q_act - prev_pos) / TIME_STEP;
        int tau_dis = disturbance_observer1(&lp_filter, vel);

        // Calculate tau
        // tau = tau_des + tau_dis
        // tau_des = an*q2_des;
        int tau = AN * q2_des + tau_dis;

        // Update disturbance observer
        disturbance_observer2(tau);

        motor.setTorque(&motor, tau);

        // Wait for the descrete timestep. Substract the time spent calculating
        prev_pos = enc.getValue(&enc);
        sleep(TIME_STEP - getCalculationTime());
    }

    return 0;
}

int getCalculationTime(void)
{
    // Return time spent on:
    // * Obtaining measurements
    // * Data processing
    // * Algorithm processing
    // * Actuator control

    // Najlakše bi bilo da nam ovo i nije potrebno
    return 0; // Neka ostane 0 za MVP.
}

void sleep(int time)
{
}

void ISR_pb1(void) // endstop 1 reached
{
    enc.setValue(&enc, 0);
    // Recalibrate encoder, null position
}

void ISR_pb2(void) // endstop 2 reached
{
    enc.setValue(&enc, ENDSTOP2_VALUE);
}