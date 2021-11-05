#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "LEDarray.h"
#include "ADC.h"
#include "timers.h"
#include "interrupts.h"
#include "LCD.h"


#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) 
{
    // setup pin for output (connected to LED)
    LATHbits.LATH3=0;   //set initial output state
    TRISHbits.TRISH3=0; //set TRIS value for pin (output)
   
    // setup pin to track & visualise each second
    LATDbits.LATD7=0;   //set initial output state
    TRISDbits.TRISD7=0; //set TRIS value for pin (output)
    
    // Initialise scripts 
    LEDarray_init();
    ADC_init();
    Timer0_init();
    Interrupts_init();
    LCD_Init();
    
    // Variables for function use (not to be tempered with)
    unsigned int light_strength=0;      // For ADC value measurement function use
    unsigned int temp=0;                // For clock function use 
    unsigned int secs=0;                // Increments clock 
    unsigned int leap_year=0;           // For Leap Year function use 
    unsigned int monthdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    unsigned int daylight_flag = 0;     // Ensures Daylight Savings adjustment done once
    unsigned int dawn_dusk[2] = {0,0};  // Stores sunrise and sunset time
    char buf[25];                       // For LCD Function use 
    
    // Preset Variables Here 
    unsigned int set_brightness=50; // Set brightness level at which LED comes on 
    int minutes=12;                  // Set current time in mins
    unsigned int hour=14;            // Set current time in hours (24 hour clock)
    unsigned int day_of_week = 5;   // Set Day of the week, 1 for Mon etc.)
    unsigned int daydate = 5;      // Set Current Day date
    unsigned int month = 11;         // Set Current Month 
    unsigned int year = 2021;       // Set Current Year
 
    while (1) {
        light_strength = ADC_getval();
        if (light_strength >= set_brightness || (1<hour && hour<5 ) ) { // still bright out OR between 1-5am
            LATHbits.LATH3 = 0;} // Toggle off LED
        else {LATHbits.LATH3 = 1;} // Toggle on LED
        
        // Adjusting for Leap Year - Calendar
        leap_year = year % 4; 
        if (leap_year == 0) { monthdays[1] = 29;} 
        else { monthdays[1] = 28;}
        
        // In-built Clock, Checks RD7 if a second has passed and resets flags accordingly
        if (LATDbits.LATD7 != temp) { secs += 1; temp = LATDbits.LATD7;} 
        if (secs >= 60) { minutes += 1; secs = 0;}
        if (minutes >= 60) { hour += 1; minutes = 0;}
        if (hour >= 24) {hour = 0; daydate += 1; day_of_week+=1;}
        if (day_of_week > 7) {day_of_week=1;}
        if (month > 12) { year += 1; month = 1; daydate=1;}  
        if (daydate > monthdays[month-1]) { month += 1; daydate = 1; 
            dawn_dusk[0]=0; dawn_dusk[1]=0;}  // Resets Sun Synchronisation flag for the month
        if (month>12) { year+=1; month=1; daydate=1;}  
        LEDarray_disp_bin(hour);
        
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
                    if (daylight_flag==0 && hour==1){ // First time for the Day
                        hour-=1;
                        daylight_flag=1; // Ensures only done once for day
                    }
                }
            }
        }
       
        // Synchronise with Sunlight monthly
        if (daydate == 25){ // checked once a month
            if (dawn_dusk[0] == 0){ // If not done yet
                if (light_strength >= set_brightness){
                    if (hour>5 && hour<9){
                        dawn_dusk[0] = hour*60 + minutes;  
                    } 
                } 
            }
            if (dawn_dusk[1] == 0){
                if (light_strength <= set_brightness){
                    if (hour>5 && hour<9){
                        dawn_dusk[1] = hour*60 + minutes;
                    } 
                } 
            }
            unsigned int midtime = (dawn_dusk[0]+dawn_dusk[1])/2;
            unsigned int midhour = midtime/60;
            unsigned int midmin = midtime-(midhour*60);
            int diff = 720 - midtime;  
            unsigned int curtime = (hour*60) + minutes;
            curtime += diff;
            hour = curtime/60;
            minutes = curtime - (hour*60);      
        }
        
        // LCD Visualisation code
        sprintf(buf,"%d:%d %d-%d-%d",hour,minutes,daydate,month,year);
        LCD_sendbyte(0b00001100,0); //Turn on display
        LCD_sendstring(buf); //Send string to LCD
        __delay_ms(500); //Delay to show value before clearing
        LCD_sendbyte(0b00000001,0); //Clear display
        __delay_ms(2);
    }
}
