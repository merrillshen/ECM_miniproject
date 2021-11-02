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
    
    // Initialise scripts 
    LEDarray_init();
    ADC_init();
    Timer0_init();
    Interrupts_init();
    
    // Variables for function use (not to be set)
    unsigned int light_strength=0;
    unsigned int temp=0;
    unsigned int secs=0; 
    unsigned int leap_year=0; 
    unsigned int monthdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    unsigned int daylight_flag = 0;
    
    // Preset Variables Here 
    unsigned int set_brightness=50; // Set brightness level at which LED comes on 
    unsigned int minutes=0;         // Set current time in mins
    int hour=0;                     // Set current time in hours (24 hour clock)
    unsigned int day_of_week = 0;   // Day of the week, 1 for Mon etc.)
    unsigned int daydate = 0;       // Day date
    unsigned int month = 0;         // Month date 
    unsigned int year = 0;          // Year date
 
    while (1) {
        light_strength = ADC_getval();
        if (light_strength >= set_brightness || (1<hour && hour<5 ) ) { // still bright out or between 1-5am
            LATHbits.LATH3 = 0; }// Toggle off LED
        else {LATHbits.LATH3 = 1;} // Toggle on LED
        
        // In-built Clock, Checks RD7 if a second has passed
        if (LATDbits.LATD7 != temp) { secs += 1; temp = LATDbits.LATD7;}
        if (secs == 60) { minutes += 1; secs = 0;}
        if (minutes == 60) { hour += 1; minutes = 0;}
        if (hour == 24) {hour = 0;daydate += 1; day_of_week+=1;}
        if (day_of_week > 7) {day_of_week=1;}
        LEDarray_disp_bin(hour);
        
        // Adjusting for Leap Year - Calendar
        leap_year = year % 4; 
        if (leap_year == 0) { monthdays[1] = 29;} 
        else { monthdays[1] = 28;}
        if (daydate > monthdays[month-1]) { month += 1; daydate = 1; daylight_flag=0;}  
        if (month > 12) { year += 1; month = 1;}  
        
        // // Adjusting Daylight Savings - Calendar
        if (day_of_week == 7) {
            if ((daydate+7) > monthdays[month-1]) { // Check if last Sunday of Month
                if (month == 3){
                    if (daylight_flag==0){ // First time for the Day
                        hour+=1;
                        daylight_flag=1; // Ensures only done once for day
                    }
                }
                if (month ==10) {
                    if (daylight_flag==0){ // First time for the Day
                        hour-=1;
                        daylight_flag=1; // Ensures only done once for day
                    }
                }
            }
        }
    }
}
