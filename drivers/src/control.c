#include "control.h"
#include "lp_filter.h"
#include "main.h"

static signed long prev_tau = 0;
static signed long prev_error = 0;
ControlerConf global_conf;
// LP filter
static LowPassFilter lp_filter;


void control_init(ControlerConf *conf)
{
    global_conf.KP = conf->KP;
    global_conf.KI = conf->KI;
    global_conf.KD = conf->KD;
    global_conf.G = conf->G;
    global_conf.An = conf->An;
    // *** Filter ***
    filter_init(&lp_filter, FILTER_ALPHA, 0);
}

signed long pd_control(signed long error)
{
    // Calculate control output
    signed long output = global_conf.KP * error + global_conf.KD * (error - prev_error);

    // Update previous error
    prev_error = error;
    return output;
}


signed long disturbance_observer1(signed long vel)
{
    signed long value = prev_tau + global_conf.G*global_conf.An*vel;
    return apply_filter(&lp_filter, value) - global_conf.G*global_conf.An*vel;
}

void disturbance_observer2(signed long tau)
{
    prev_tau = tau;

}
