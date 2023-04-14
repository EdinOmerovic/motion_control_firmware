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

#define BLINKY_LED_GPIO 31

// Control parameters
#define KP_val 1
#define KI_val 1
#define KD_val 1

#define AN 1    // Manipulator inertia, mass
#define G_val 1 // filter parameters
// Absolute value of the possition when touching endstop 2
#define ENDSTOP2_VALUE 83500// 8.35cm



#endif /* MAIN_H_ */
