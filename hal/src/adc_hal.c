#include "adc_hal.h"
#include "F28x_Project.h"
#include "F2837xD_Examples.h" // Provjeri da li treba dodavati i ovo

//
// Globals
//
static Uint16 AdcaResult0;
static Uint16 AdcaResult1;
static Uint16 AdcbResult0;
static Uint16 AdcbResult1;

#define EX_ADC_RESOLUTION 12

void ConfigADC()
{
    //write configurations
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    //AdcbRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4


    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    //AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    //Set pulse positions to late
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    //AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    //power up the ADCs
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    //AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;

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

    //AdcaRegs.ADCSOC1CTL.bit.CHSEL = 3;  //SOC1 will convert pin A3 //sample window is acqps +
    //AdcaRegs.ADCSOC1CTL.bit.ACQPS = acqps;
                                           //1 SYSCLK cycles
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 1; //end of SOC1 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared

    //ADCB
    //AdcbRegs.ADCSOC0CTL.bit.CHSEL = 2;  //SOC0 will convert pin B2
    //AdcbRegs.ADCSOC0CTL.bit.ACQPS = acqps; //sample window is acqps +
                                           //1 SYSCLK cycles
    //AdcbRegs.ADCSOC1CTL.bit.CHSEL = 3;  //SOC1 will convert pin B3
    //AdcbRegs.ADCSOC1CTL.bit.ACQPS = acqps; //sample window is acqps +
                                           //1 SYSCLK cycles

    //AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 1; //end of SOC1 will set INT1 flag
    //AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    //AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
}



//
// ConfigureADC - Write ADC configurations and power up the ADC for both
//                ADC A and ADC B
//
void ConfigureADC(void)
{
    ConfigADC();
}


Uint16 readAnalog()
{
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    //convert, wait for completion, and store results
    //start conversions immediately via software, ADCA
    AdcaRegs.ADCSOCFRC1.all = 0x00001; //SOC0 and SOC1

    //wait for ADCA to complete, then acknowledge flag
    while(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 0);
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    AdcaResult0 = AdcaResultRegs.ADCRESULT0;
    return AdcaResult0;
}


//
// End of file
//
