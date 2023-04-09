#include "F28x_Project.h"
#include "F2837xD_Cla_typedefs.h"  // F2837xD CLA Type definitions
#include "F2837xD_device.h"        // F2837xD Headerfile Include File
#include "F2837xD_Examples.h"      // F2837xD Examples Include File

// GPIO
//#define BLINKY_LED_GPIO 31

// QEP:
#include "encoder_hal.h"
POSSPEED qep_posspeed = POSSPEED_DEFAULTS;
Uint16 Interrupt_Count = 0;

//
// Function Prototypes
//
void initEpwm();
__interrupt void prdTick(void);

// Zadatak:
// Ukljuci i probaj sve module koji su ti neophodni za rad

// Ubaciti QEP modul

void main(void)
{
    InitSysCtrl();

    // GPIO
    //InitGpio();
    //GPIO_SetupPinMux(BLINKY_LED_GPIO, GPIO_MUX_CPU1, 0);
    //GPIO_SetupPinOptions(BLINKY_LED_GPIO, GPIO_OUTPUT, GPIO_PUSHPULL);
    //GPIO_WritePin(BLINKY_LED_GPIO, 1);

    // QEP
    //
    // For this case only init GPIO for eQEP1 and ePWM1
    // This function is found in F2837xD_EQep.c
    //
    InitEQep1Gpio();
    InitEPwm1Gpio();
    EALLOW;
    GpioCtrlRegs.GPADIR.bit.GPIO4 = 1; // GPIO4 as output simulates Index signal
    GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;  // Normally low
    EDIS;

    //
    // Step 3. Clear all __interrupts and initialize PIE vector table:
    // Disable CPU __interrupts
    //
    DINT;

    //
    // Initialize the PIE control registers to their default state.
    // The default state is all PIE __interrupts disabled and flags
    // are cleared.
    // This function is found in the F2837xD_PieCtrl.c file.
    //
    InitPieCtrl();

    //
    // Disable CPU __interrupts and clear all CPU __interrupt flags:
    //
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the __interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in F2837xD_DefaultIsr.c.
    // This function is found in F2837xD_PieVect.c.
    //
    InitPieVectTable();

    //
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    //
    EALLOW;
    // This is needed to write to EALLOW protected registers
    PieVectTable.EPWM1_INT = &prdTick;
    EDIS;

    //
    // Step 5. User specific code, enable __interrupts:
    // Enable CPU INT1 which is connected to CPU-Timer 0:
    //
    IER |= M_INT3;

    //
    // Enable TINT0 in the PIE: Group 3 __interrupt 1
    //
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;

    //
    // Enable global Interrupts and higher priority real-time debug events:
    //
    EINT;
    // Enable Global __interrupt INTM
    ERTM;
    // Enable Global realtime __interrupt DBGM

    qep_posspeed.init(&qep_posspeed);
}

//
// prdTick - EPWM1 Interrupts once every 4 QCLK counts (one period)
//
__interrupt void prdTick(void)
{
    Uint16 i;

    //
    // Position and Speed measurement
    //
    qep_posspeed.calc(&qep_posspeed);

    //
    // Control loop code for position control & Speed control
    //
    Interrupt_Count++;

    //
    // Every 1000 __interrupts(4000 QCLK counts or 1 rev.)
    //
    if (Interrupt_Count == 1000)
    {
        EALLOW;
        GpioDataRegs.GPASET.bit.GPIO4 = 1; // Pulse Index signal  (1 pulse/rev.)
        for (i = 0; i < 700; i++)
        {
        }
        GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;
        Interrupt_Count = 0;               // Reset count
        EDIS;
    }

    //
    // Acknowledge this __interrupt to receive more __interrupts from group 1
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    EPwm1Regs.ETCLR.bit.INT = 1;
}
