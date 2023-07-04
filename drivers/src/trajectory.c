#include "trajectory.h"
#include "trajectory_lut.h"

#include "motor.h" //for map
#include "main.h" // for ENDSTOP values

// NOTE: Given trajectory always has to be positive and in within the range of the actual manipulator workspace (ENDSTOP1, ENDSTOP2)

#define FIXED_VALUE 0
#define ANALOG_READ_MAX_VALUE 4095
#define MAX_BUFFER_VALUE 3

static source_e src_config;

// The first value (index = 0) is always the newest sample:
static Uint32 previous_trajectory_value_buffer[MAX_BUFFER_VALUE];


// Used for the lookup table
//static Uint32 counter = 0;
static Uint32 fixed_step_value = STEPS_VALUE_1;

signed long unit_sine(Uint32 sample_nr, Uint32 freq_scale);

void trajectory_init(source_e src_conf)
{
    src_config = src_conf;
    switch (src_conf)
    {
    case ANALOG_READ:
        // ADC
        // Check if the ADC is not initialized, and initialize it.
        // For now we know it's initialized.
        break;

    case HARDCODED:
        // Initialize the values array

        // Make it possible to have an array in memory containing the recording of a desired trajectory. Going in a loop.
        // When the loop finishes, the movement stops, and the setup waits for a signal from a user
        break;

    case FIXED:
        // Make it possible to feed in the data.
        // How to synchronize the fed in data with the execution and reading of it?
        break;
    case STEPS:
        break;

    case SINE:
        break;

    default:
        break;
    }
}

Uint32 _obtainTrajectoryValue(Uint32 sample_nr)
{
    switch (src_config)
    {
    case ANALOG_READ:
        return map(readAnalog(), 0,  ANALOG_READ_MAX_VALUE, 0, ENDSTOP2_VALUE);
    case HARDCODED:
        // Get the next sample in the array
        return 20*sineLookupTable[(sample_nr) % NR_ELEMENTS];
    case FIXED:
        return FIXED_VALUE;
    case STEPS:
        if (sample_nr % STEP_TIME_DURATION == 0){
            // Switch to different set position
            fixed_step_value = (STEPS_VALUE_1 + STEPS_VALUE_2) - fixed_step_value;
        }
        return fixed_step_value;

    case SINE:
        // Read the analog pot and determine the sine frequency
        return SINE_AMPLITUDE*sineLookupTable[(sample_nr/map(readAnalog(), 0,  ANALOG_READ_MAX_VALUE, 0, SINE_MAX_FREQ)) % NR_ELEMENTS] + SINE_BIAS;

    default:
        // If everything else fails, go to starting position.
        return ENDSTOP1_VALUE;
    }
}

void push_back_fixed_size_array(Uint32 *arr, Uint32 size, Uint32 new_element) {
    Uint32 i;
    for (i = size - 1; i >= 1; i--) {
        arr[i] = arr[i-1];  // Shift each element one index to the right
    }
    arr[0] = new_element;   // Place the new element at the front of the array
}

Uint32 getTrajectory(Uint32 sample_nr)
{
    // Obtain the newest position value
    Uint32 new_pos = _obtainTrajectoryValue(sample_nr);

    // Update the position buffer so that the new position is the fist element.
    push_back_fixed_size_array(previous_trajectory_value_buffer, MAX_BUFFER_VALUE, new_pos);

    // Return the newest position value
    return new_pos;
}



// For now this is hardcoded as a utilization of central differences formula for 2 order time-derivative:
// It would be nice if we had a general derivative block to use.
signed long getTrajectory2od(void)
{
    signed long finite_diff = previous_trajectory_value_buffer[0] - 2*previous_trajectory_value_buffer[1] + previous_trajectory_value_buffer[2];
    return (finite_diff / (TIME_STEP*TIME_STEP)); // in um/s^2
}
