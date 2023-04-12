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
    EQep1Regs.QUPRD = 2000000;            // Unit Timer for 100Hz at 200 MHz
                                          // SYSCLKOUT
    EQep1Regs.QDECCTL.bit.QSRC = 00;      // QEP quadrature count mode
    EQep1Regs.QEPCTL.bit.FREE_SOFT = 2;
    EQep1Regs.QEPCTL.bit.PCRM = 00;       // PCRM=00 mode - QPOSCNT reset on
                                          // index event
    EQep1Regs.QEPCTL.bit.UTE = 1;         // Unit Timeout Enable
    EQep1Regs.QEPCTL.bit.QCLM = 1;        // Latch on unit time out
    EQep1Regs.QPOSMAX = 0xffffffff;
    EQep1Regs.QEPCTL.bit.QPEN = 1;        // QEP enable
    EQep1Regs.QCAPCTL.bit.UPPS = 5;       // 1/32 for unit position
    EQep1Regs.QCAPCTL.bit.CCPS = 6;       // 1/64 for CAP clock
    EQep1Regs.QCAPCTL.bit.CEN = 1;        // QEP Capture Enable
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
