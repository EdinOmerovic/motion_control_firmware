#ifndef CONTROL_H
#define CONTROL_H
#include "lp_filter.h"

typedef struct
{
    int KP;
    int KI;
    int KD;
    int G;
    int An;
} ControlerConf;

void control_init(ControlerConf *);

int pd_control(int error);

int disturbance_observer1(LowPassFilter *, int);

int disturbance_observer2(int, int);

#endif