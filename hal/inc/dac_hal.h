
#ifndef DAC_HAL_H
#define DAC_HAL_H
#include "F2837xD_device.h"

//
// Globals
//
volatile static struct DAC_REGS* DAC_PTR[4] = {0x0,&DacaRegs,&DacbRegs,&DaccRegs};
static Uint16 dacval = 2048;

//
// Defines
//
#define REFERENCE_VDAC      0
#define REFERENCE_VREF      1
#define DACA         1
#define DACB         2
#define DACC         3
#define REFERENCE            REFERENCE_VDAC
#define DAC_NUM                DACA

//
// Function Prototypes
//
void configureDAC(Uint16 dac_num);

#endif // DAC_HAL_H
