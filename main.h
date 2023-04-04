#ifndef MAIN_H
#define MAIN_H
#include "encoder.h"
#include "motor.h"
#include "trajectory.h"
#include "control.h"
#include "lp_filter.h"

#define TIME_STEP 1000 // [ns]

// Control parameters
#define KP_val 1
#define KI_val 1
#define KD_val 1

#define AN 1    // Manipulator inertia, mass
#define G_val 1 // filter parameters
// Absolute value of the possition when touching endstop 2
#define ENDSTOP2_VALUE 83500000 // 8.35cm 



// ****** FUNCTION PROTOTYPES ******

int getCalculationTime(void);

void sleep(int time);

void ISR_pb1(void);

void ISR_pb2(void);

#endif