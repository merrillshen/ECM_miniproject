#include <xc.h>
#include "LEDarray.h"

/************************************
/ LEDarray_init
/ Function used to initialise pins to drive the LEDarray
************************************/
void LEDarray_init(void)
{   
    //set up TRIS registers for pins connected to LED array
    //set initial output LAT values (they may have random values when powered on)
    
    LATGbits.LATG0=0;   //set initial output state
    TRISGbits.TRISG0=0;
    
    LATGbits.LATG1=0;   //set initial output state
    TRISGbits.TRISG1=0;

    LATAbits.LATA2=0;   //set initial output state
    TRISAbits.TRISA2=0;	
	
    LATFbits.LATF6=0;   //set initial output state
    TRISFbits.TRISF6=0;
    
    LATAbits.LATA4=0;   //set initial output state
    TRISAbits.TRISA4=0;	
    
    LATAbits.LATA5=0;   //set initial output state
    TRISAbits.TRISA5=0;	
    
    LATFbits.LATF0=0;   //set initial output state
    TRISFbits.TRISF0=0;
    
    LATBbits.LATB0=0;   //set initial output state
    TRISBbits.TRISB0=0;
    
    LATBbits.LATB1=0;   //set initial output state
    TRISBbits.TRISB1=0;
}

/************************************
/ LEDarray_disp_bin
/ Function used to display a number on the LED array in binary
************************************/
void LEDarray_disp_bin(unsigned int number)
{
	//some code to turn on/off the pins connected to the LED array
	//if statements and bit masks can be used to determine if a particular pin should be on/off
	//see Readme.md for examples
    if (number & 0b000000001) { LATGbits.LATG0=1;} else { LATGbits.LATG0=0;} //1
    if (number & 0b000000010) { LATGbits.LATG1=1;} else { LATGbits.LATG1=0;} //2
    if (number & 0b000000100) { LATAbits.LATA2=1;} else { LATAbits.LATA2=0;} //4
    if (number & 0b000001000) { LATFbits.LATF6=1;} else { LATFbits.LATF6=0;} //8
    if (number & 0b000010000) { LATAbits.LATA4=1;} else { LATAbits.LATA4=0;} //16
    if (number & 0b000100000) { LATAbits.LATA5=1;} else { LATAbits.LATA5=0;} //32
    if (number & 0b001000000) { LATFbits.LATF0=1;} else { LATFbits.LATF0=0;} //64
    if (number & 0b010000000) { LATBbits.LATB0=1;} else { LATBbits.LATB0=0;} //128
    if (number & 0b100000000) { LATBbits.LATB1=1;} else { LATBbits.LATB1=0;} //256
}

void buttonpress_init(void)
{  
    // setup pin for input (connected to left button)
    TRISFbits.TRISF2=1; //set TRIS value for pin (input)
    ANSELFbits.ANSELF2=0; //turn off analogue input on pin 
}

/************************************
/ Function LEDarray_disp_dec
/ Used to display a number on the LEDs
/ where each LED is a value of 10
************************************/
void LEDarray_disp_dec(unsigned int number)
{
	unsigned int disp_val;
	disp_val = 0;
	//some code to manipulate the variable number into the correct
	//format and store in disp_val for display on the LED array
    if (number > 20) { disp_val += 1;} 
    if (number > 25) { disp_val += 2;}
    if (number > 29) { disp_val += 4;} 
    if (number > 39) { disp_val += 8;}
    if (number > 49) { disp_val += 16;} 
    if (number > 59) { disp_val += 32;} 
    if (number > 69) { disp_val += 64;} 
    if (number > 79) { disp_val += 128;}
    if (number > 89) { disp_val += 256;}
	LEDarray_disp_bin(disp_val); 	//display value on LED array
}


/************************************
/ LEDarray_disp_PPM
/ Function used to display a level on the LED array with peak hold
/ cur_val is the current level from the most recent sample, and max is the peak value for the last second
/ these input values need to calculated else where in your code
************************************/
void LEDarray_disp_PPM(unsigned int cur_val, unsigned int max)
{
	unsigned int disp_val;
    disp_val = 0;
    if (cur_val > 29) { disp_val += 1;} 
    if (cur_val > 39) { disp_val += 2;}
    if (cur_val > 49) { disp_val += 4;} 
    if (cur_val > 59) { disp_val += 8;}
    if (cur_val > 69) { disp_val += 16;} 
    if (cur_val > 79) { disp_val += 32;} 
    if (cur_val > 89) { disp_val += 64;} 
    if (cur_val > 99) { disp_val += 128;}
    if (cur_val > 109) { disp_val += 256;}
    
	if (cur_val >= max) { LEDarray_disp_bin(disp_val);} //display value on LED array
    else {
        if (29 >= max && max > 1) { disp_val += 1;} 
        if (39 >= max && max > 29) { disp_val += 2;}
        if (49 >= max && max > 39) { disp_val += 4;} 
        if (59 >= max && max > 49) { disp_val += 8;}
        if (69 >= max && max > 59) { disp_val += 16;} 
        if (79 >= max && max > 69) { disp_val += 32;} 
        if (89 >= max && max > 79) { disp_val += 64;} 
        if (99 >= max && max > 89) { disp_val += 128;}
        if (119 >= max &&max > 99) { disp_val += 256;}
        LEDarray_disp_bin(disp_val);
    }
	// some code to format the variable cur_val and max, store in disp_val for display on the LED array
	// hint: one method is to manipulate the variables separately and then combine them using the bitwise OR operator
}