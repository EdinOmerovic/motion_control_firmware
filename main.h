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
#define PROCESSING_TIME 6 // [us]


#define LOOP_CLOCK_GPIO 32
#define BLINKY_LED_GPIO 31

// Control parameters
#define KP_val 1
#define KD_val 10

#define AN 1    // Manipulator inertia, mass
#define G_val 1
// filter parameters 0 - 1000
#define FILTER_ALPHA 995

// Absolute value of the position when touching end-stop 2
#define ENDSTOP1_VALUE 0
#define ENDSTOP2_VALUE 42500

#define ENCODER_STARTING_POSITION 0

#define MAX_MOTOR_TAU 40000
#define MIN_MOTOR_TAU -40000

// *Hard-coded: supplied using lookup table
// *Analog: obtained using analog read
// *Fixed: fixed value of the trajectory
#define TRAJECTORY_SELECT ANALOG_READ




#endif /* MAIN_H_ */
