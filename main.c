// TODO: double-check values
// NOTE: all values are represented in the following units:
// * length = micrometers [um]
// * time  = microseconds [us]
// * mass = grams [g]

// Contains project level configuration
#include "main.h"

// Code Composer Project related stuff
#include "helpers.h"
#include "F28x_Project.h"

#include "serial.h"
#include "sysctl.h"

// Drivers
#include "encoder.h"
#include "motor.h"
#include "trajectory.h"
#include "control.h"


// **** INSTATATION ****
Encoder enc;
Motor motor;
POSSPEED qep_module = POSSPEED_DEFAULTS;

// Local function definitions
void initSystem(void);
void controlLoop(void);
__interrupt void ISR_pb1(void);// Triggered by end-stop 1
__interrupt void ISR_pb2(void);// Triggered by end-stop 2


// ***** Static variables used in the control loop *****
static Uint32 start_time;
/* Current sample number */
static Uint32 sample_nr = 0;
/* Previous position value */
static Uint32 q_previous = ENCODER_STARTING_POSITION;
/* Target position value */
static Uint32 q_ref = 0;
/* Second derivative of target position */
static signed long q2_ref = 0;
/* Actual (measured) position value */
static Uint32 q_act = 0;
/* Difference between actual and target*/
static signed long error = 0;
/* Desired acceleration */
static signed long q2_des = 0;
/* Actual actuator velocity */
static signed long vel = 0;
/* Disturbance torque value */
static signed long tau_dis = 0;
/* Output torque value */
static signed long tau = 0;

void main(void)
{
    // ***** Initialize system and low-level components *****
    initSystem();

    // ----- Initialization of high level components -----
    // *** Encoder ***
    EncoderConf enc_conf =
    {
        .scalingFactor = 50, // scale to get micrometers
        .startingValue = ENCODER_STARTING_POSITION // starting value should be determined by auto-homing: ENDSTOP1_VALUE
    };

    encoder_init(&enc);
    enc.configure(&enc, &enc_conf, &qep_module);

    // *** Controller ***
    ControlerConf controler_conf =
    {   .An = AN,
        .G = G_val,
        .KP = KP_val,
        .KD = KD_val,
        .filter_alpha = FILTER_ALPHA
    };
    control_init(&controler_conf);

    // *** Trajectory ***
    trajectory_init(TRAJECTORY_SELECT);

    // *** Motor ***
    MotorConf motor_conf =
    {
         .scaler = 1,
         .tau_offset = 0,
         .voltage_offset = 0
    };
    motor_init(&motor, &motor_conf);

    // ----- END of initialization of high level components -----


    // Enable protection for writing to protected registers
    EDIS;
    SCIprintf("INIT-COMPLETE\n");
    StartCpuTimer0();
    DELAY_US(100); // Timer stabilisation delay
    while(1)
    {
        start_time = CpuTimer0Regs.TIM.all;
        // Diode used for timing performance
        GPIO_WritePin(LOOP_CLOCK_GPIO, 1);
        controlLoop();
        SCIprintf("%u;%u;%u;%d;%d\n", sample_nr, q_ref, q_act, tau_dis, tau);
        sample_nr++;
        GPIO_WritePin(LOOP_CLOCK_GPIO, 0);

        // Get timer value
        ReloadCpuTimer0();
        DELAY_US((TIME_STEP - (start_time - CpuTimer0Regs.TIM.all + FIXED_TIME_OFFSET)));

    }
}

// Main control loop
// FIXME: check actual values of the position
// TODO: provjeri da li je frekvencija stvarno 200MHz ili 100MHz
void controlLoop(void)
{
    // Check end-stop values
    if (GPIO_ReadPin(0) == 0){
        enc.setValue(&enc, 0);
    }else if (GPIO_ReadPin(1) == 0){
        enc.setValue(&enc, ENDSTOP2_VALUE);
    }


    // Get the desired trajectory:
    q_ref = getTrajectory();

    // Get the second derivative of the desired trajectory
    q2_ref = getTrajectory2od(); // in [m/s^2

    // Get current absolute position from encoder (in micrometers)
    // The position is ranging form 0 to ENDSTOP2_VALUE
    q_act = enc.getValue(&enc);

    // Calculate position error
    error = q_ref - q_act;

    // Calculate second derivative of q desired
    // FIXME: division to get rid of high freq. noise
    q2_des = (q2_ref / 1000000) + pd_control(error);

    // Disturbance observer
    // Calculate velocity:
    // v = delta X / delta T
    // FIXME: remove magic constant
    vel = (q_act - q_previous) / (TIME_STEP / 1000); // in um/s
    tau_dis = disturbance_observer1(vel);

    // Calculate tau
    // tau = tau_des + tau_dis
    // tau_des = an*q2_des;
    // Saturation add logic
    // FIXME: remove magic constant
    saturated_add_3(&tau, AN * q2_des, tau_dis/1000);

    // Update disturbance observer
    disturbance_observer2(tau);

    motor.setTorque(&motor, saturate(-1*tau, MIN_MOTOR_TAU, MAX_MOTOR_TAU));

    q_previous = q_act;
}


// Interrupt service routines triggered on falling edge signal on end-stop GPIOs, useful for stuff.
__interrupt void ISR_pb1(void)
{
    //enc.setValue(&enc, 0);
    // Additionally cut all power to the motor driver
    // Acknowledge this interrupt to get more from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
__interrupt void ISR_pb2(void)
{
    //enc.setValue(&enc, ENDSTOP2_VALUE);
    // Acknowledge this interrupt to get more from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

// Setup the GPIO interrupt
void initSystem(void)
{
    // *** Pre-configuration ***
    // Clear all __interrupts and initialize PIE vector table:
    IFR = 0x0000;
    // Disable CPU __interrupts
    DINT;
    // This is needed to write to EALLOW protected registers
    EALLOW;

    // FIXME: paljenje ovoga PRAVI PROBLEM U DEBUGIRANJU
    // Set the clocking to run from the PLL at 50MHz
    //
    //SysCtl_setClock(SYSCTL_OSCSRC_OSC2 | SYSCTL_PLL_ENABLE | SYSCTL_IMULT(10) | SYSCTL_SYSDIV(2));
    //SysCtl_setAuxClock(SYSCTL_OSCSRC_OSC2 | SYSCTL_PLL_ENABLE | SYSCTL_IMULT(12) | SYSCTL_SYSDIV(2));        //60 MHz


    // **** Initialize bare-minimum embedded system ****
    InitSysCtrl();

    // *** Initialize peripheral vector interrupt table
    // Initialize the PIE control registers to their default state.
    // The default state is all PIE __interrupts disabled and flags are cleared.
    InitPieCtrl();

    // Initialize the PIE vector table with pointers to the shell Interrupt Service Routines (ISR).
    // This will populate the entire table
    // The shell ISR routines are found in F2837xD_DefaultIsr.c.
    // This function is found in F2837xD_PieVect.c.
    InitPieVectTable();

    // **** Initialize HAL components ****

    // *** GPIO ****
    InitGpio();

    // * LOOP_CLOCK_GPIO (default GPIO32) - used for loop signaling
    GPIO_SetupPinMux(LOOP_CLOCK_GPIO, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(LOOP_CLOCK_GPIO, GPIO_OUTPUT, GPIO_PUSHPULL);

    // * (ENDSTOP1 and ENSTOP2) GPIO0 and GPIO1 are inputs, triggering interrupt routine
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 0;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO0 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;

    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 0;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;

    // GPIO0 is XINT1, GPIO1 is XINT2
    GPIO_SetupXINT1Gpio(0);
    GPIO_SetupXINT2Gpio(1);

    // Configure XINT1
    XintRegs.XINT1CR.bit.POLARITY = 0;          // Falling edge interrupt
    XintRegs.XINT2CR.bit.POLARITY = 0;          // Falling edge interrupt

    // Enable XINT1 and XINT2
    XintRegs.XINT1CR.bit.ENABLE = 1;            // Enable XINT1
    XintRegs.XINT2CR.bit.ENABLE = 1;            // Enable XINT2

    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // Enable PIE Group 1 INT4
    PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          // Enable PIE Group 1 INT5

    // **** Initialize other CPU components ****

    // *** Timers ***
    InitCpuTimers();

    // Timer is used for measuring time
    // This determines the timer overflow time.
    ConfigCpuTimer(&CpuTimer0, 200, TIME_STEP);
    CpuTimer0Regs.TPR.all = 0xC7; // 200 prescaler to get time difference in us

    // *** DAC ***
    configureDAC(DAC_NUM);

    // *** ADC ***
    ConfigureADC();


    // *** Serial ***
    ConfigureUART();

    // Remap ISR functions to user interrupts.
    EALLOW;

    //PieVectTable.TIMER0_INT = &controlLoop;
    PieVectTable.XINT1_INT = &ISR_pb1;
    PieVectTable.XINT2_INT = &ISR_pb2;

    // Set peripheral interrupt enable
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
    IER |= M_INT1; // Enable CPU INT1

    // *** Post-configuration ***
    // Disable write protection
    EDIS;
    // Enable Global __interrupt INTM
    EINT;
    // Enable global Interrupts and higher priority real-time debug events:
    ERTM;
}

