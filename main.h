/*
 * main.h
 *
 *  Created on: 12 Apr 2023
 *      Author: Master
 */

#ifndef MAIN_H_
#define MAIN_H_

// Should be bigger than 1000 for now
#define TIME_STEP 1000 // [us]
#define FIXED_TIME_OFFSET 20 // [us]


#define LOOP_CLOCK_GPIO 32
#define BLINKY_LED_GPIO 31

// Control parameters
#define KP_val 20
#define KD_val 1

#define AN 1    // Manipulator inertia, mass
#define G_val 1
// filter parameters 0 - 1000
#define FILTER_ALPHA 995

// Absolute value of the position when touching end-stop 2
#define ENDSTOP1_VALUE 0
#define ENDSTOP2_VALUE 42500

#define ENCODER_STARTING_POSITION 0

#define MAX_MOTOR_TAU 30000
#define MIN_MOTOR_TAU -30000

// *** SELECT THE TRAJECTORY INPUT ***
// *HARDCODED: supplied using lookup table
// *ANALOG_READ: obtained using analog read
// *STEPS: alternating step of specified amplitude (start: STEPS_VALUE_1, stop: STEPS_VALUE_2)
// *SINE: sine wave of controllable frequency, using the analog input (potentiometer)
// *FIXED: fixed value of the trajectory
#define TRAJECTORY_SELECT ANALOG_READ


// Used in measuring the execution time:
#define TIMER0_PERIOD_US 1


// Step trajectory parameters
#define STEP_TIME_DURATION 5000 // 5s
#define STEPS_VALUE_1 100
#define STEPS_VALUE_2 5000

// Sine trajectory parameters
#define SINE_AMPLITUDE 20
#define SINE_BIAS 1000
#define SINE_MAX_FREQ 20



#endif /* MAIN_H_ */
