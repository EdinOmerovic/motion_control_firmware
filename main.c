// Implement high level trajectory tracking algorithm using the low-level drivers/libraries

#include <stdio.h>

#include "encoder.h"
#include "motor.h"
#include "trajectory.h"
#include "control.h"

#define TIME_STEP 1000 // 1000[ns]

int getCalculationTime(void)
{
    // Return time spent on:
    // * Obtaining measurements
    // * Data processing
    // * Algorithm processing
    // * Actuator control

    return 0;
}

int main()
{
    // **** Instantation ****
    Encoder *enc;


    // // **** INITIALIZATION *****
    
    // control_init(KP, KI, KD);
    
    // encoder_init(&enc);
     
    // // **** CONFIGURATION ****
    // enc->configure();



    // while (1)
    // {
    //     // Get current position from encoder
    //     current_pos = enc->encoder_get_position();

    //     // Get the current trajectory: 
    //     int target_pos = getTrajectory();

    //     // Calculate error
    //     error = target_pos - current_pos;

    //     // Calculate control output
    //     control_output = KP * error + KD * (error - prev_error);

    //     // Update previous error
    //     prev_error = error;

    //     // Wait for the descrete timestep. Substract the time spent calculating
    //     sleep(TIME_STEP - getCalculationTime());
    // }

    return 0;
}