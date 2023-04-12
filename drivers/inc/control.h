#ifndef CONTROL_H
#define CONTROL_H
#include "lp_filter.h"

typedef struct
{
    Uint32 KP;
    Uint32 KI;
    Uint32 KD;
    Uint32 G;
    Uint32 An;
} ControlerConf;

void control_init(ControlerConf *);

Uint32 pd_control(int error);

Uint32 disturbance_observer1(LowPassFilter *, int);

void disturbance_observer2(int);

#endif
