#include "interrupt.h"
#include "mcc.h"
void  INTERRUPT_Initialize (void)
{
    RCONbits.IPEN = 0;
    ADCON1 = 0x0F;
    IPR1bits.TMR1IP = 0;
}

void __interrupt () INTERRUPT_InterruptManager (void)
{
    if(PIE1bits.TMR1IE == 1 && PIR1bits.TMR1IF == 1)
    {
        TMR1_ISR();
    }
    else
    {
        //Unhandled Interrupt
    }
}