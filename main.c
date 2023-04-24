// NOTE: all values are represented in the following units:
// * lenght = micrometers [us]
// * time  = microseconds [us]
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

// Global static values
POSSPEED qep_module = POSSPEED_DEFAULTS;

// **** INSTATATION ****
// Encoder
        Encoder enc;

// Motor
        Motor motor;

// Local function definitions
//__interrupt void prdTick(void);
//__interrupt void controlLoop(void); // Called every N us
void initSystem(void);
void controlLoop(void);
__interrupt void ISR_pb1(void);// Triggered by end-stop 1
__interrupt void ISR_pb2(void);// Triggered by end-stop 2

// TODO
// Provjeri na kojem tacno izlazu ti je DACA
// Podesi da sve jedinice budu odgovarajuce

void main(void)
{
    // Initialize system and low-level components
    initSystem();

    // Initialization of high level components
    // *** Encoder ***
    encoder_init(&enc);
    // scaling factor = 100 to get micrometers
    // starting value should be determined by auto-homing: ENDSTOP1_VALUE
    EncoderConf enc_conf =
    {
        .scalingFactor = 100,
        .startingValue = 5000
    };

    enc.configure(&enc, &enc_conf, &qep_module);

    // *** Controller ***
    ControlerConf controler_conf =
    {   .An = AN,
        .G = G_val,
        .KP = KP_val,
        .KI = KI_val,
        .KD = KD_val,
    };

    control_init(&controler_conf);

    // *** Trajectory ***
    // *Hardcoded: supplied using lookup table
    // *Analog: obtained using analog read
    // *Fixed: fixed value of the trajectory
    // Ovdje je takoder potrebno specificirati koje su dimenzije sistema
    trajectory_init(ANALOG_READ);

    // *** Motor ***
    MotorConf motor_conf =
    {   .scaler = 1, .tau_offset = 0, .voltage_offset = 0};
    motor_init(&motor, &motor_conf);

    // ******* END of initialization of high level components

    // Start the TIMER0 - controlToop trigger
    //CpuTimer0Regs.TCR.bit.TSS = 0;

    // Enable protection for writing to protected registers
    EDIS;

    while(1)
    {
        controlLoop();
        DELAY_US((TIME_STEP - PROCESSING_TIME));
    }
}

void saturated_add_3(signed long *result, signed long a, signed long b)
{
    if (a > 0 && b > 0)
    {
        Uint64 temp_res = (Uint32)a + (Uint32)b;
        if (temp_res > 4200000 - 1)
        {
            *result = 4200000;
        }
        else
        {
            signed long res = (signed long)a + (signed long)b;
            *result =  res;
        }
    }else
    {
        // Implement underflow protection
        *result = a + b;
    }
}

// Main control loop
static Uint32 prev_pos = 0;
static Uint32 q_ref = 0;
static Uint32 q_act = 0;
static signed long q2_ref = 0;
static signed long error = 0;
static signed long q2_des = 0;
static signed long vel = 0;
static signed long tau_dis = 0;
static signed long tau = 0;
void controlLoop(void)
{
    // Diode used for debugging
    GPIO_WritePin(LOOP_CLOCK_GPIO, 1);

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
    q2_des = (q2_ref / 1000000) + pd_control(error);

    // Disturbance observer
    // Calculate velocity:
    // v = delta X / delta T
    vel = (q_act - prev_pos) / (TIME_STEP / 1000); // in um/s
    tau_dis = disturbance_observer1(vel);

    // Calculate tau
    // tau = tau_des + tau_dis
    // tau_des = an*q2_des;
    // Saturation add logic
    saturated_add_3(&tau, AN * q2_des, tau_dis/1000);

    // Update disturbance observer
    disturbance_observer2(tau);

    motor.setTorque(&motor, tau);

    prev_pos = q_act;

    // Acknowledge this __interrupt to receive more __interrupts from group 1
    //CpuTimer0.InterruptCount++;
    //PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    //CpuTimer0Regs.TCR.bit.TIF = 1; // Clear the timer interrupt flag

    // Diode used for debugging
    GPIO_WritePin(LOOP_CLOCK_GPIO, 0);

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
    // *** Preconfiguration ***
    // Clear all __interrupts and initialize PIE vector table:
    IFR = 0x0000;
    // Disable CPU __interrupts
    DINT;
    // This is needed to write to EALLOW protected registers
    EALLOW;

    // **** Initialize bare-minimum embedded system ****
    InitSysCtrl();

    // *** Initialize peripheral vector interrupt table
    // Initialize the PIE control registers to their default state.
    // The default state is all PIE __interrupts disabled and flags are cleared.
    InitPieCtrl();

    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the __interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in F2837xD_DefaultIsr.c.
    // This function is found in F2837xD_PieVect.c.
    InitPieVectTable();

    // **** Initialize HAL components ****

    // *** GPIO ****
    InitGpio();

    // * LOOP_CLOCK_GPIO (default GPIO32) - used for loop signaling
    // Just a regular output
    GPIO_SetupPinMux(LOOP_CLOCK_GPIO, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(LOOP_CLOCK_GPIO, GPIO_OUTPUT, GPIO_PUSHPULL);

    // * (ENDSTOP1 and ENSTOP2) GPIO0 and GPIO1 are inputs, triggering interrupt routine
    // FIXME: create a function for setting up the inputs like this
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;         // GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 0;          // input
    GpioCtrlRegs.GPAQSEL1.bit.GPIO0 = 0;        // XINT1 Synch to SYSCLKOUT only

    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;         // GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 0;          // input
    GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 0;        // XINT2 Qual using 6 samples

    // GPIO0 is XINT1, GPIO1 is XINT2
    GPIO_SetupXINT1Gpio(0);
    GPIO_SetupXINT2Gpio(1);

    // Configure XINT1
    XintRegs.XINT1CR.bit.POLARITY = 0;          // Falling edge interrupt
    XintRegs.XINT2CR.bit.POLARITY = 0;          // Falling edge interrupt

    // Enable XINT1 and XINT2
    XintRegs.XINT1CR.bit.ENABLE = 1;            // Enable XINT1
    XintRegs.XINT2CR.bit.ENABLE = 1;            // Enable XINT2

    // TODO provjeri da li je ovome mjesto ovdje
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // Enable PIE Group 1 INT4
    PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          // Enable PIE Group 1 INT5

    // **** Initialize other CPU components ****

    // *** Timers ***
    //InitCpuTimers();
    // Configure CPU-Timer 0 to __interrupt
    //ConfigCpuTimer(&CpuTimer0, 200, TIME_STEP);

    // To ensure precise timing, use write-only instructions to write to the entire
    // register. Therefore, if any of the configuration bits are changed in
    // ConfigCpuTimer and InitCpuTimers (in F2837xD_cputimervars.h), the below
    // settings must also be updated.
    //CpuTimer0Regs.TCR.all = 0x4001;
    // Enable TINT0 in the PIE: Group 1 __interrupt 7
    //PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    // Enable CPU INT1 which is connected to CPU-Timer 0:
    //IER |= M_INT1;

    // *** DAC ***
    configureDAC(DAC_NUM);

    // *** ADC ***
    ConfigureADC();

    // Remap ISR functions to user interrupts.
    EALLOW;
    //PieVectTable.TIMER0_INT = &controlLoop;
    PieVectTable.XINT1_INT = &ISR_pb1;
    PieVectTable.XINT2_INT = &ISR_pb2;

    // Set peripheral interrupt enable
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;

    // *** Postconfiguration ***
    // Disable write protection
    EDIS;
    // Enable Global __interrupt INTM
    EINT;
    // Enable global Interrupts and higher priority real-time debug events:
    ERTM;
}

