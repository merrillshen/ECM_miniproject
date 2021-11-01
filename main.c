#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "LEDarray.h"
#include "ADC.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) 
{
    // setup pin for output (connected to LED)
    LATHbits.LATH3=0;   //set initial output state
    TRISHbits.TRISH3=0; //set TRIS value for pin (output)
    
    unsigned int light_strength=0;
    LEDarray_init();
    ADC_init();
    unsigned int set_brightness=50;
    while (1) {
        light_strength = ADC_getval();
        if (light_strength >= set_brightness) { // still bright out
            LATHbits.LATH3 = 0; //toggle off LED
        }
        else {
            LATHbits.LATH3 = 1; //toggle on LED
        }
    }
}
