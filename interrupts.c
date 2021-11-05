#include <xc.h>
#include "interrupts.h"

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/
void Interrupts_init(void)
{
	// turn on global interrupts, peripheral interrupts and the interrupt source 
	// It's a good idea to turn on global interrupts last, once all other interrupt configuration is done.
    INTCONbits.PEIE=1; 	//enable peripheral interrupts
    PIE0bits.TMR0IE=1; // enable timer0 interrupt
    PIE2bits.C1IE=1;    //enable Comparator 1 interrupt bit (interrupt source)
    INTCONbits.GIE=1; 	//enable interrupts globally 
}

/************************************
 * High priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
************************************/
void __interrupt(high_priority) HighISR()
{   //add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...
    if (PIR2bits.C1IF == 1){ 
        LATHbits.LATH3 = !LATHbits.LATH3; //toggle LED
        PIR2bits.C1IF=0; //toggle off flag
    }
    
    if (PIR0bits.TMR0IF == 1){ //timer flag
        LATDbits.LATD7 = !LATDbits.LATD7; //toggle LED, could use other bits but using LED for easy viewing 
        TMR0H=0b00001011;
        TMR0L=0b11011011;
        PIR0bits.TMR0IF=0; //toggle off flag
    }
}

