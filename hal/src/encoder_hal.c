//###########################################################################
//
// FILE:    Example_posspeed.c
//
// TITLE:   Pos/speed measurement using EQEP peripheral
//
// DESCRIPTION:

// Included Files
#include "F28x_Project.h"
#include "encoder_hal.h"


// POSSPEED_Init - Initialize EQEP1 configuration
void POSSPEED_Init(void)
{
    EALLOW;
    // TODO: speed calculation can be done using encoder data
    //EQep1Regs.QUPRD = 2000000;            // Unit Timer for 100Hz at 200 MHz

    // QEP quadrature count mode
    EQep1Regs.QDECCTL.bit.QSRC = 00;

    EQep1Regs.QEPCTL.bit.FREE_SOFT = 2;
    EQep1Regs.QEPCTL.bit.PCRM = 01;       // PCRM=00 mode - QPOSCNT reset on maximum event
    EQep1Regs.QEPCTL.bit.QCLM = 0;        // Latch on CPU read

    // Initial counter value, set to middle.
    EQep1Regs.QPOSINIT = ENC_INIT_VALUE;
    EQep1Regs.QPOSMAX = 0xffffffff;
    // Software enable
    EQep1Regs.QEPCTL.bit.SWI = 1;


    EQep1Regs.QEPCTL.bit.QPEN = 1;        // QEP enable
   // Used for velocity calculation
    //EQep1Regs.QCAPCTL.bit.UPPS = 5;       // 1/32 for unit position
    // EQep1Regs.QCAPCTL.bit.CCPS = 6;       // 1/64 for CAP clock
    // EQep1Regs.QCAPCTL.bit.CEN = 1;        // QEP Capture Enable
    EDIS;
}

Uint32 POSSPEED_Read(POSSPEED *p)
{

    p->previous_value = p->current_value;
    p->current_value = EQep1Regs.QPOSCNT;

    // 0 - counter-clockwise (back)
    // 1 - clockwise (forward)
    p->direction = EQep1Regs.QEPSTS.bit.QDF;


    return (p->current_value / p->scaling_factor);
}
