//Define Device 
#ifdef _18F8722
#pragma config  OSC = HSPLL
#pragma config 	FCMEN = OFF
#pragma config 	IESO = OFF
#pragma config 	PWRT = OFF
#pragma config 	BOREN = OFF
#pragma config 	WDT = OFF
#pragma config 	MCLRE = ON
#pragma config 	LVP = OFF
#pragma config 	XINST = OFF
#endif
//**************************************

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
//Define port
#define DfIbutton TRISAbits.TRISA5
#define Ibutton PORTAbits.RA5
#define DfDbutton  TRISBbits.TRISB0
#define Dbutton PORTBbits.RB0
#define press 0
//Define parameters
#define NORMAL_MODE 100
#define FAST_MODE 300
#define NORMAL_TIME 50
#define FAST_TIME 10
//Define FSM
#define BEGIN 0
#define INCREASE_PORTD 1
#define INCREASE_100MS 2
#define INCREASE_300MS 3
#define DECREASE_PORTD 4
#define DECREASE_100MS 5
#define DECREASE_300MS 6

unsigned int state = BEGIN;
//Define variable for read Buttons
int pRB0 = 0;
int pRB0_1s = 0;
int pRB0_3s = 0;
int pRA5 = 0;
int pRA5_1s = 0;
int pRA5_3s = 0;
int pCountRA5 = 0;
int pCountRB0 = 0;
int countRA5_100ms = 0;
int countRA5_300ms = 0;
int countRB0_100ms = 0;
int countRB0_300ms = 0;
//Reload the timer
volatile uint16_t timerReloadVal;
//Checking the stable of the Button
int firstReadRA5;
int secondReadRA5;
int firstReadRB0;
int secondReadRB0;

unsigned char normalCountUpFlag = 0;
unsigned char fastCountUpFlag = 0;
unsigned char normalCountDownFlag = 0;
unsigned char fastCountDownFlag = 0;

void OSCILLATOR(void) {
    OSCCON = 0x76; //Set internal clock = 8MHz/4 
    OSCTUNE = 0x00;
}

void PIN_MANAGER(void) {
    DfIbutton = 1;
    DfDbutton = 1;
    TRISD = 0x00;
    LATD = 0x00;
}

void INTERRUPT() {
    RCONbits.IPEN = 0;
    IPR1bits.TMR1IP = 0;
}

void ADC(void) {
    ADCON1 = 0x0F;
}
void TMR1_Start(void);
void TMR1_Stop(void);

void TMR1_IN(void) {
    T1CON = 0x7D; //Set pre-scale = 8, enable internal clock = 2MHz / 8 = 250kHz
    /*
     Normally, Timer1 count from 0 to FFFF,
     * Assume that we need a clock of x (s), then we need f = x.internal_clock cycles before interrupt
     * So {TMR1H:TMR1L} = FFFF - f
     * 
     */
    TMR1H = 0xF6;
    TMR1L = 0x3B;
    timerReloadVal = TMR1;
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 1;
    TMR1_Start();
}

void TMR1_Reload(void) {
    // Write to the Timer1 register
    TMR1H = (timerReloadVal >> 8);
    TMR1L = (uint8_t) timerReloadVal;
}

void TMR1_Start(void) {
    T1CONbits.TMR1ON = 1;
}

void TMR1_Stop(void) {
    T1CONbits.TMR1ON = 0;
}

void SYSTEM_IN() {
    OSCILLATOR();
    PIN_MANAGER();
    ADC();
    INTERRUPT();
    TMR1_IN();
}

int readButtonRB0() {
    firstReadRB0 = secondReadRB0;
    secondReadRB0 = PORTBbits.RB0;
    if (firstReadRB0 == secondReadRB0)
        if (firstReadRB0 == 0) return 1;
    return 0;
}

int readButtonRA5() {
    firstReadRA5 = secondReadRA5;
    secondReadRA5 = PORTAbits.RA5;
    if (firstReadRA5 == secondReadRA5)
        if (firstReadRA5 == 0) return 1;
    return 0;
}

void TMR1_ISR() {
    PIR1bits.TMR1IF = 0;
    TMR1_Reload();
    if (readButtonRA5()) {
        pCountRB0 = 0;
        pRA5 = 1;
        pCountRA5++;
        if (pCountRA5 > FAST_MODE) {
            pRA5_1s = 0;
            pRA5_3s = 1;
            countRA5_300ms++;
            if (countRA5_300ms >= FAST_TIME) {
                fastCountUpFlag = 1;
                countRA5_300ms = 0;
            }
        } else if (pCountRA5 > NORMAL_MODE) {
            pRA5_3s = 0;
            pRA5_1s = 1;
            countRA5_100ms++;
            if (countRA5_100ms >= NORMAL_TIME) {
                normalCountUpFlag = 1;
                countRA5_100ms = 0;
            }
        }
    } else if (readButtonRB0()) {
        pCountRA5 = 0;
        pRB0 = 1;
        pCountRB0++;
        if (pCountRB0 > FAST_MODE) {
            pRB0_1s = 0;
            pRB0_3s = 1;
            countRB0_300ms++;
            if (countRB0_300ms > FAST_TIME) {
                fastCountDownFlag = 1;
                countRB0_300ms = 0;
            }

        } else if (pCountRB0 > NORMAL_MODE) {
            pRB0_3s = 0;
            pRB0_1s = 1;
            countRB0_100ms++;
            if (countRB0_100ms >= NORMAL_TIME) {
                normalCountDownFlag = 1;
                countRB0_100ms = 0;
            }
        }
    } else {
        pCountRA5 = 0;
        pCountRB0 = 0;
        pRA5 = 0;
        pRA5_1s = 0;
        pRA5_3s = 0;
        pRB0 = 0;
        pRB0_1s = 0;
        pRB0_3s = 0;
        countRA5_100ms = 0;
        countRA5_300ms = 0;
        countRB0_100ms = 0;
        countRB0_300ms = 0;
    }
}

void __interrupt() CheckButtonsPress() {
    if (PIE1bits.TMR1IE == 1 && PIR1bits.TMR1IF == 1) {
        TMR1_ISR();
    } else {

    }
}
//RA5 is higher priority than RB0
void checkRA5() {
    if (pRA5) {
        LATD++;
        state = INCREASE_PORTD;
    }
}

void FSM() {
    switch (state) {
        case BEGIN:
            if (pRA5) {
                LATD++;
                state = INCREASE_PORTD;
            } else if (pRB0) {
                LATD--;
                state = DECREASE_PORTD;
            }
            break;
        case INCREASE_PORTD:
            if (!pRA5) state = BEGIN;
            else if (pRA5_1s) state = INCREASE_100MS;
            break;
        case INCREASE_100MS:
            if (normalCountUpFlag == 1) {
                LATD++;
                normalCountUpFlag = 0;
            }
            if (!pRA5) state = BEGIN;
            if (pRA5_3s) state = INCREASE_300MS;
            break;
        case INCREASE_300MS:
            if (fastCountUpFlag == 1) {
                fastCountUpFlag = 0;
                LATD++;
            }
            if (!pRA5) state = BEGIN;
            break;
        case DECREASE_PORTD:
            if (!pRB0) state = BEGIN;
            else if (pRB0_1s) state = DECREASE_100MS;
            checkRA5();
            break;
        case DECREASE_100MS:
            if (normalCountDownFlag == 1) {
                normalCountDownFlag = 0;
                LATD--;
            }
            if (!pRB0) state = BEGIN;
            if (pRB0_3s) state = DECREASE_300MS;
            checkRA5();
            break;
        case DECREASE_300MS:
            if (fastCountDownFlag == 1){
                fastCountDownFlag = 0;
                LATD--;
            }
            if (!pRB0) state = BEGIN;
            checkRA5();
            break;
    }
}

int main() {
    //Set up here
    SYSTEM_IN();
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

    while (1) {
        FSM();
    }
    return 0;
}
