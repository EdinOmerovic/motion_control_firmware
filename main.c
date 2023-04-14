// NOTE: all values are represented in the follwing units:
// * lenght = micrometers.
// * time  = microseconds
// * mass = grams

// Code Composer Project related stuff
#include "F28x_Project.h"

// Contains project level configuration
#include "main.h"

// Drivers
#include "encoder.h"
#include "motor.h"
#include "trajectory.h"
#include "control.h"
#include "lp_filter.h"

// Global static values
POSSPEED qep_module = POSSPEED_DEFAULTS;
Uint32 prev_pos = 0;

// **** INSTATATION ****
// Encoder
Encoder enc;

// Controller
ControlerConf controler_conf;

// Motor
Motor motor;

// LP filter
LowPassFilter lp_filter;

// Local function definitions
//__interrupt void prdTick(void);
__interrupt void controlLoop(void); // Called every N us
__interrupt void ISR_pb1(void); // Triggered by end-stop 1
__interrupt void ISR_pb2(void); // Triggered by end-stop 2
void initSystem(void);

// TODO
// Provjeri na kojem tacno izlazu ti je DACA
// Naprav da se glavna petlja poziva timiranom interrupt rutinom
// Podesi da sve jedinice budu odgovarajuce

void main(void)
{
    // Initialize system and low-level components
    initSystem();

    // Initialization of high level components
    // Encoder
    encoder_init(&enc);

    // scaling factor = 100 to get micrometers
    // starting value should be determined by autohoming: ENDSTOP1_VALUE
    EncoderConf enc_conf = { .scalingFactor = 100, .startingValue = 0 };
    enc.configure(&enc, &enc_conf, &qep_module);

    // Controller
    controler_conf.An = AN;
    controler_conf.G = G_val;
    controler_conf.KP = KP_val;
    controler_conf.KI = KI_val;
    controler_conf.KD = KD_val;
    control_init(&controler_conf);

    // Desired trajectory:
    // *Hardcoded: supplied using lookup table
    // *Analog: obtained using analog read
    // *Fixed: fixed value of the trajectory
    trajectory_init(FIXED);

    filter_init(&lp_filter, G_val, 0);


    MotorConf motor_conf = {.scaler = 1, .tau_offset = 0, .voltage_offset = 0};
    motor_init(&motor, &motor_conf);

    EALLOW;
    CpuTimer0Regs.TCR.bit.TSS = 0; // Start the timer
    EDIS;

    // Handle everything in controlLoop(void)
    for (;;);
}

// Main control loop
__interrupt void controlLoop(void)
{
    // Diode used for debugging
    GPIO_WritePin(BLINKY_LED_GPIO, 1);
    // Get the desired trajectory:
    Uint32 q_ref = getTrajectory();

    // FIXME:
    // Get the second derivative of the desired trajectory
    Uint32 q2_ref = getTrajectory2od(); // Za prvi MVP to je 0

    // Get current absolute position from encoder (in micrometers)
    Uint32 q_act = enc.getValue(&enc);

    // Calculate position error
    signed long error = q_ref - q_act;

    // Calculate second derivative of q desired
    signed long q2_des = q2_ref + pd_control(error);

    // Disturbance observer
    // Calculate velocity:
    // v = delta X / delta T
    signed long vel = (q_act - prev_pos) / TIME_STEP; // in um/us
    signed long tau_dis = disturbance_observer1(&lp_filter, vel);

    // Calculate tau
    // tau = tau_des + tau_dis
    // tau_des = an*q2_des;
    signed long tau = AN * q2_des + tau_dis;

    // Update disturbance observer
    disturbance_observer2(tau);

    motor.setTorque(&motor, tau);

    prev_pos = enc.getValue(&enc);

    // Acknowledge this __interrupt to receive more __interrupts from group 1
    CpuTimer0.InterruptCount++;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    //CpuTimer0Regs.TCR.bit.TIF = 1; // Clear the timer interrupt flag

    // Diode used for debugging
    GPIO_WritePin(BLINKY_LED_GPIO, 0);

}

__interrupt void ISR_pb1(void)
{
    enc.setValue(&enc, 0);

    // Additionally cut all power to the motor driver
    // Acknowledge this interrupt to get more from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
__interrupt void ISR_pb2(void)
{

    enc.setValue(&enc, ENDSTOP2_VALUE);
    // Acknowledge this interrupt to get more from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}

// Setup the GPIO interrupt
void initSystem(void)
{
    InitSysCtrl();

    // Initialize HAL components
    // GPIO
    InitGpio();
    // Blue led
    GPIO_SetupPinMux(BLINKY_LED_GPIO, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(BLINKY_LED_GPIO, GPIO_OUTPUT, GPIO_PUSHPULL);

    // GPIO0 and GPIO1 are inputs
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;         // GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 0;          // input
    GpioCtrlRegs.GPAQSEL1.bit.GPIO0 = 0;        // XINT1 Synch to SYSCLKOUT only

    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;         // GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 0;          // input
    GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 0;        // XINT2 Qual using 6 samples
    EDIS;

    // GPIO0 is XINT1, GPIO1 is XINT2
    GPIO_SetupXINT1Gpio(0);
    GPIO_SetupXINT2Gpio(1);

    // Configure XINT1
    XintRegs.XINT1CR.bit.POLARITY = 0;          // Falling edge interrupt
    XintRegs.XINT2CR.bit.POLARITY = 0;          // Falling edge interrupt

    // Enable XINT1 and XINT2
    XintRegs.XINT1CR.bit.ENABLE = 1;            // Enable XINT1
    XintRegs.XINT2CR.bit.ENABLE = 1;            // Enable XINT2

    // DAC
    configureDAC(DAC_NUM);
    // ADC
    ConfigureADC(); // configure and power up
    SetupADCSoftware(); //Setup the ADCs for software conversions

    // Step 3. Clear all __interrupts and initialize PIE vector table:
    // Disable CPU __interrupts
    DINT;

    // Initialize the PIE control registers to their default state.
    // The default state is all PIE __interrupts disabled and flags
    // are cleared.
    // This function is found in the F2837xD_PieCtrl.c file.
    InitPieCtrl();

    // Disable CPU __interrupts and clear all CPU __interrupt flags:
    IER = 0x0000;
    IFR = 0x0000;

    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the __interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in F2837xD_DefaultIsr.c.
    // This function is found in F2837xD_PieVect.c.
    InitPieVectTable();

    // This is needed to write to EALLOW protected registers
    EALLOW;
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    PieVectTable.TIMER0_INT = &controlLoop;
    PieVectTable.XINT1_INT = &ISR_pb1;
    PieVectTable.XINT2_INT = &ISR_pb2;

    // Step 4. Initialize the Device Peripheral.
    InitCpuTimers();   // For this example, only initialize the Cpu Timers

    // Configure CPU-Timer 0 to __interrupt
    ConfigCpuTimer(&CpuTimer0, 200, TIME_STEP);

    // To ensure precise timing, use write-only instructions to write to the entire
    // register. Therefore, if any of the configuration bits are changed in
    // ConfigCpuTimer and InitCpuTimers (in F2837xD_cputimervars.h), the below
    // settings must also be updated.
    CpuTimer0Regs.TCR.all = 0x4001;

    // Enable TINT0 in the PIE: Group 1 __interrupt 7
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // Enable PIE Group 1 INT4
    PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          // Enable PIE Group 1 INT5

    // Enable CPU INT1 which is connected to CPU-Timer 0:
    IER |= M_INT1;
    // Disable write protection
    EDIS;
    // Enable global Interrupts and higher priority real-time debug events:
    EINT;
    // Enable Global __interrupt INTM
    ERTM;
    // Enable Global realtime __interrupt DBGM
}

