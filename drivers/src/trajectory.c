#include "trajectory.h"
#include "trajectory_lut.h"

#define FIXED_VALUE 0

static source_e src_config;

static int trajectory_value;
static int counter = 0;

void trajectory_init(source_e src_conf)
{
    src_conf = src_conf;
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
    
}