/***
 * This library should provide a way to obtain absolute trajectory:
 * ** Whether that being hardcoded function
 * ** or trajectory obtained from measurements (analogRead)
 * ** or trajectory communicated from somewhere
*/

#ifndef TRAJECTORY_H
#define TRAJECTORY_H

typedef enum {
    ANALOG_READ,
    HARDCODED,
    FIXED
} source_e;

void trajectory_init(source_e);

int getTrajectory(void);

int getTrajectory2od(void);

#endif
