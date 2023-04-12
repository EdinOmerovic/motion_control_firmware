//###########################################################################
//
// FILE:    Example_posspeed.h
//
// TITLE:   Pos/speed measurement using EQEP peripheral
//
// DESCRIPTION:
//
// Header file containing data type and object definitions and
// initializers.


#ifndef __POSSPEED__
#define __POSSPEED__

#include "IQmathLib.h"
#include "F2837xD_device.h"

//
// Defines
//
#define POSSPEED_DEFAULTS {0x0, 0x0, 1, 0,\
                           (void (*)(long))POSSPEED_Init,\
                           (Uint32 (*)(long))POSSPEED_Read}

//
// Globals
//
typedef struct
{
    Uint32 current_value;
    Uint32 previous_value;
    Uint32 scaling_factor;
    bool direction;
    // Function pointers
    void (*init)();
    Uint32 (*read)();
} POSSPEED;
typedef POSSPEED *POSSPEED_handle;

void POSSPEED_Init(void);

Uint32 POSSPEED_Read(POSSPEED_handle);

#endif //  __POSSPEED__
