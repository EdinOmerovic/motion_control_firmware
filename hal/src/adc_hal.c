#include "F28x_Project.h"
#include "adc_hal.h"
#include "F2837xD_sysctrl.h"

#define EX_ADC_RESOLUTION 16

void ConfigADC()
{
    EALLOW;

    //write configurations
    // set the bit to enable the desired ADC clock PCLKCR13
    CpuSysRegs.CPUSYSLOCK1.bit.PCLKCR13 = 1;

    // Set the desired ADC clock devider in the PRESCALE field of ADCCTL2
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4

    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    //Set pulse positions to late
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    //power up the ADCs
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    //delay for 1ms to allow ADC time to power up
    DELAY_US(1000);


    Uint16 acqps;

    // Determine minimum acquisition window (in SYSCLKS) based on resolution
    if(ADC_RESOLUTION_12BIT == AdcaRegs.ADCCTL2.bit.RESOLUTION)
    {
        acqps = 14; //75ns
    }
    else //resolution is 16-bit
    {
        acqps = 63; //320ns
    }

    //Select the channels to convert and end of conversion flag
    //ADCA
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 2;  //SOC0 will convert pin A2 sample window is acqps + 1 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps;

    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
}


// ConfigureADC - Write ADC configurations and power up the ADC for both
void ConfigureADC(void)
{
    ConfigADC();
}


Uint16 readAnalog()
{
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; // Clear INT1 flag for ADCA

    //convert, wait for completion, and store results

    //start conversions immediately via software
    AdcaRegs.ADCSOCFRC1.bit.SOC0 = 1; //SOC0

    //wait for ADCA to complete, then acknowledge flag
    while(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 0);
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    return AdcaResultRegs.ADCRESULT0;
}


//
// End of file
//
