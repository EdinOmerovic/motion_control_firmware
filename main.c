// Code Composer Project related stuff
#include "F28x_Project.h"
#include "F2837xD_device.h"  // F2837xD CLA Type definitions
#include "F2837xD_Examples.h"      // F2837xD Examples Include File

// Contains project level configuration
// Project defines
#include "main.h"

// Hardware abstraction layer stuff
#include "encoder_hal.h"
#include "dac_hal.h"

// Drivers
#include "encoder.h"
#include "motor.h"
#include "trajectory.h"
#include "control.h"
#include "lp_filter.h"

// Global static values
POSSPEED qep_posspeed = POSSPEED_DEFAULTS;
Uint16 Interrupt_Count = 0;
Uint16 dacval = 4095;
int prev_pos = 0;

// **** INSTATATION ****
// Encoder
Encoder enc;
EncoderConf enc_conf;

// Controler
ControlerConf controler_conf;

// Motor
Motor motor;
MotorConf motor_conf;

// LP filter
LowPassFilter lp_filter;

// Local function definitions
//__interrupt void prdTick(void);
__interrupt void controlLoop(void); // Called every N us
__interrupt void ISR_pb1(void); // Triggered by endstop 1
__interrupt void ISR_pb2(void); // Triggered by endstop 2

// TODO: Skontaj kako da na izlazu generises napon zeljenog nivoa.
// Napravi da enkoder radi u isto vrijeme
// Napravi da mozes da imas interrupte na GPIO pinovima
// Skaliranje enkoderskih vrijednosti

// Probao sam encoder + dac
// Nisam probao ADC
// Provjeri na kojem tacno izlazu ti je DACA
// Naprav da se glavna petlja poziva timiranom interrupt rutinom

void main(void)
{
    InitSysCtrl();

    // Initialize HAL components
    // GPIO
    InitGpio();
    //GPIO_SetupPinMux(BLINKY_LED_GPIO, GPIO_MUX_CPU1, 0);
    //GPIO_SetupPinOptions(BLINKY_LED_GPIO, GPIO_OUTPUT, GPIO_PUSHPULL);
    //GPIO_WritePin(BLINKY_LED_GPIO, 1);
    // QEP
    InitEQep1Gpio();
    //InitEPwm1Gpio();
    EALLOW;
    GpioCtrlRegs.GPADIR.bit.GPIO4 = 1; // GPIO4 as output simulates Index signal
    GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;  // Normally low
    EDIS;
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

    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    EALLOW;
    // This is needed to write to EALLOW protected registers
    // PieVectTable.EPWM1_INT = &prdTick;
    EDIS;

    // Step 5. User specific code, enable __interrupts:
    // Enable CPU INT1 which is connected to CPU-Timer 0:
    IER |= M_INT3;

    // Enable TINT0 in the PIE: Group 3 __interrupt 1
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;

    // Enable global Interrupts and higher priority real-time debug events:
    EINT;
    // Enable Global __interrupt INTM
    ERTM;
    // Enable Global realtime __interrupt DBGM

    // Initialization of high level components
    // Encoder
    encoder_init(&enc);
    enc_conf.startingValue = 0;
    enc_conf.scalingFactor = 1000;
    enc_conf.absoluteDimentsions = 0;
    enc.configure(&enc, &enc_conf);

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

    motor_init(&motor, &motor_conf);

    // QEP
    qep_posspeed.init(&qep_posspeed);

    while (1)
    {
        // Read analog
        //readAnalog();

        // Read encoder value, scale it, and send to analog
        qep_posspeed.read(&qep_posspeed);

        // Uint16 value = qep_posspeed.theta_mech/SCALER;

        // Push to DAC
        DAC_PTR[DAC_NUM]->DACVALS.all = qep_posspeed.theta_mech / SCALER;
        DELAY_US(2);
    }
}

// Main control loop
__interrupt void controlLoop(void)
{
    // Get the desired trajectory:
    int q_ref = getTrajectory();

    // FIXME: dodaj da se osnovu funkcije može odrediti 2 izvod trajektorije
    // Get the second derivative of the desired trajectory
    int q2_ref = getTrajectory2od(); // Za prvi MVP to je 0

    // Get current absolute position from encoder
    int q_act = enc.getValue(&enc);

    // Calculate position error
    int error = q_ref - q_act;

    // Calculate second derivative of q desired
    int q2_des = q2_ref + pd_control(error);

    // Disturbance observer
    // Calculate velocity:
    // v = delta X / delta T
    int vel = (q_act - prev_pos) / TIME_STEP;
    int tau_dis = disturbance_observer1(&lp_filter, vel);

    // Calculate tau
    // tau = tau_des + tau_dis
    // tau_des = an*q2_des;
    int tau = AN * q2_des + tau_dis;

    // Update disturbance observer
    disturbance_observer2(tau);

    motor.setTorque(&motor, tau);

    // Wait for the descrete timestep. Substract the time spent calculating
    prev_pos = enc.getValue(&enc);

}

//
// prdTick - EPWM1 Interrupts once every 4 QCLK counts (one period)
//
/*__interrupt void prdTick(void)
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
 }*/

