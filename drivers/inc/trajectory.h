/***
 * This library should provide a way to obtain absolute trajectory:
 * ** Whether that being hardcoded function
 * ** or trajectory obtained from measurements (analogRead)
 * ** or trajectory communicated from somewhere
*/

#ifndef TRAJECTORY_H
#define TRAJECTORY_H
#include "adc_hal.h"


typedef enum {
    ANALOG_READ,
    HARDCODED,
    FIXED,
    STEPS
} source_e;

void trajectory_init(source_e);

Uint32 getTrajectory(Uint32 sample_nr);

signed long getTrajectory2od(void);

#endif
