#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "LEDarray.h"
#include "ADC.h"
#include "timers.h"
#include "interrupts.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) 
{
    // setup pin for output (connected to LED)
    LATHbits.LATH3=0;   //set initial output state
    TRISHbits.TRISH3=0; //set TRIS value for pin (output)
   
    // setup pin to track each second
    LATDbits.LATD7=0;   //set initial output state
    TRISDbits.TRISD7=0; //set TRIS value for pin (output)
    
    // Initialise all functions 
    LEDarray_init();
    ADC_init();
    Timer0_init();
    Interrupts_init();
    
    unsigned int light_strength=0;
    unsigned int set_brightness=50; //To set brightness level at which LED comes on 
    unsigned int temp=0;
    unsigned int secs=0; 
    unsigned int minutes=0;  // To preset current time in mins
    unsigned int hour=0; // To preset current time in hours (24 hour clock)
    while (1) {
        light_strength = ADC_getval();
        if (light_strength >= set_brightness) { // still bright out
            LATHbits.LATH3 = 0; //toggle off LED
        }
        else {
            LATHbits.LATH3 = 1; //toggle on LED
        }
        
        if (LATDbits.LATD7 != temp) { //checks if a second has passed
            secs += 1;
            temp = LATDbits.LATD7;
        }
        if (secs == 60) { 
            minutes += 1;
            secs = 0;
        }
        if (minutes == 60) { 
            hour += 1;
            minutes = 0;
        }
        if (hour == 24) { 
            hour = 0;
        }
        LEDarray_disp_bin(hour);
    }
}
