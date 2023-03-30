#include "control.h"


int prev_error = 0;
int prev_tau = 0;
ControlerConf global_conf; 


void control_init(ControlerConf *conf)
{
    global_conf.KP = conf->KP;
    global_conf.KI = conf->KI;
    global_conf.KD = conf->KD;
    global_conf.G = conf->G;
    global_conf.An = conf->An;
}

int pd_control(int error)
{
    // Calculate control output
    int output = global_conf.KP * error + global_conf.KD * (error - prev_error);

    // Update previous error
    prev_error = error;
    return output;
}


int disturbance_observer1(LowPassFilter *filter, int vel)
{
    int value = prev_tau + global_conf.G*global_conf.An*vel;
    return apply_filter(filter, value) - global_conf.G*global_conf.An*vel;
}

int disturbance_observer2(int tau, int q_act)
{
    prev_tau = tau;

}