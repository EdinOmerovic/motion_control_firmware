#include "trajectory.h"
#include "trajectory_lut.h"

//TODO: implemnt second order derivative for trajectory
// for now we use fixed, so the second order derivaive of the trajectory is 0

#define FIXED_VALUE 0
//#define BUFFER_SIZE 2

static source_e src_config;

static int trajectory_value; // Used as a return value
static int counter = 0;      // Used for the lookup table

// int buffer[BUFFER_SIZE]; // Use for derivative buffer. 

void trajectory_init(source_e src_conf)
{
    src_conf = src_conf;
    switch (src_conf)
    {
    case ANALOG_READ:
        /* code */
        // Initialize analog read
        break;

    case HARDCODED:
        /* code */
        // Initialize the values array
        break;

    case FIXED:
        /* code */
        break;

    default:
        break;
    }
}

int getTrajectory(void)
{
    switch (src_config)
    {
    case ANALOG_READ:
        // Initialize analog read
        break;
    case HARDCODED:
        // Set of hardcoded values
        trajectory_value = trajectory_values[counter++ % NR_ELEMENTS];
        break;
    case FIXED:
        trajectory_value = FIXED_VALUE;
        break;

    default:
        break;
    }

    return trajectory_value;
}

int getTrajectory2od(void)
{
    if (src_config == FIXED){
        return 0; 
    }
    // TODO: Implement general case of second order derivative of trajectory.
    return 0;
}