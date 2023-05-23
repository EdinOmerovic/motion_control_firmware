/*
 * serial.c
 *
 *  Created on: 17 May 2023
 *      Author: Master
 */

#include "serial.h"
#include "sysctl.h"
#include "utils/scistdio.h"


#define SYSTEM_CLOCK_SPEED      20000000


void ConfigureUART(void)
{
    //
    // Enable UART0
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SCIA);

    //
    // Configure GPIO Pins for UART mode.
    //
    EALLOW;

    GpioCtrlRegs.GPBGMUX1.bit.GPIO43 = 3;
    GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 3;
    GpioCtrlRegs.GPBPUD.bit.GPIO43 = 0;
    GpioCtrlRegs.GPBQSEL1.bit.GPIO43 = 3;
    GpioCtrlRegs.GPBDIR.bit.GPIO43 = 0;

    /*GpioCtrlRegs.GPAGMUX2.bit.GPIO23 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 3;
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;
    GpioCtrlRegs.GPADIR.bit.GPIO23 = 0;*/


    GpioCtrlRegs.GPBGMUX1.bit.GPIO42 = 3;
    GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 3;
    GpioCtrlRegs.GPBPUD.bit.GPIO42 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO42 = 1;

    /*GpioCtrlRegs.GPAGMUX2.bit.GPIO22 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 3;
    GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;*/
    EDIS;

    //
    // Initialize the UART for console I/O.
    //
    SCIStdioConfig(SCIA_BASE, 460800, SysCtl_getLowSpeedClock(10000000));
}
