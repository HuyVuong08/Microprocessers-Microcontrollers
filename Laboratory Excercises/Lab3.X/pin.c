#include <xc.h>
#include "pin.h"

void PIN_MANAGER_Initialize(void)
{
    
    LATA = 0x00;
    TRISA = 0xF3;
//    ANSELA = 0x0B;

    LATB = 0x00;
    TRISB = 0xFF;
//    ANSELB = 0x3E;
//    WPUB = 0x00;

    LATC = 0x00;
    TRISC = 0xD7;
//    ANSELC = 0x04;

    LATD = 0x00;
    TRISD = 0x00;
//    ANSELD = 0xFF;

    LATE = 0x00;
    TRISE = 0x07;
//    ANSELE = 0x07;

    TRISAbits.TRISA5 = 1; 
    TRISBbits.TRISB0 = 1;
    INTCON2bits.nRBPU = 0x01;
}