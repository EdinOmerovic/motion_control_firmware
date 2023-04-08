#include "F28x_Project.h"
#include "F2837xD_Cla_typedefs.h"  // F2837xD CLA Type definitions
#include "F2837xD_device.h"        // F2837xD Headerfile Include File
#include "F2837xD_Examples.h"      // F2837xD Examples Include File


#define BLINKY_LED_GPIO 31

void main(void)
{
    InitSysCtrl();
    InitGpio();
    GPIO_SetupPinMux(BLINKY_LED_GPIO, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(BLINKY_LED_GPIO, GPIO_OUTPUT, GPIO_PUSHPULL);
    while(1){
        GPIO_WritePin(BLINKY_LED_GPIO, 1);
        DELAY_US(1000*500);
        // Turn off LED

        //
        GPIO_WritePin(BLINKY_LED_GPIO, 0);
        //
        DELAY_US(1000*5000);
    }

}
