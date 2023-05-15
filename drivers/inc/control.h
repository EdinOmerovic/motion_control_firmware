#ifndef CONTROL_H
#define CONTROL_H

typedef struct
{
    Uint32 KP;
    Uint32 KD;
    Uint32 G;
    Uint32 An;
    Uint32 filter_alpha;
} ControlerConf;

void control_init(ControlerConf *);

signed long pd_control(signed long);

signed long disturbance_observer1(signed long);

void disturbance_observer2(signed long);

#endif
