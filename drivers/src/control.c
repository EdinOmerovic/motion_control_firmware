#include "control.h"


signed long prev_tau = 0;
signed long prev_error = 0;
ControlerConf global_conf; 


void control_init(ControlerConf *conf)
{
    global_conf.KP = conf->KP;
    global_conf.KI = conf->KI;
    global_conf.KD = conf->KD;
    global_conf.G = conf->G;
    global_conf.An = conf->An;
}

Uint32 pd_control(int error)
{
    // Calculate control output
    Uint32 output = global_conf.KP * error + global_conf.KD * (error - prev_error);

    // Update previous error
    prev_error = error;
    return output;
}


signed long disturbance_observer1(LowPassFilter *filter, signed long vel)
{
    signed long value = prev_tau + global_conf.G*global_conf.An*vel;
    return apply_filter(filter, value) - global_conf.G*global_conf.An*vel;
}

void disturbance_observer2(signed long tau)
{
    prev_tau = tau;

}
