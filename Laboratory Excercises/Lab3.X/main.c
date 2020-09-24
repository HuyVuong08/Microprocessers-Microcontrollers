//#include <xc.h>
#include "Button.h"
#include "LCD.h"
#include "mcc.h"
#include "CLOCK.h"
#define BUTTON_PRESSED 0
#define BUTTON_RELEASED 1
unsigned char buttonStateRA5;
void buttonFSMRA5();
void buttonFSMRA5(){
    switch(buttonStateRA5){
        case BUTTON_PRESSED:
            if (readButtonRA5()) {
                buttonStateRA5 = BUTTON_RELEASED; 
                access_state = 0;
                LATD = state;
                state++;
                if (state > 5) state = 1;
    }
            break;
        case BUTTON_RELEASED:
            if(!readButtonRA5()){
                buttonStateRA5 = BUTTON_PRESSED;
            }
            break;
        default:
            break;
    }
}
void buttonFSMRB0(){
    switch(buttonStateRB0){
        case BUTTON_PRESSED:
            if (readButtonRB0()) {
                buttonStateRB0 = BUTTON_RELEASED; 
                stopWatch_state = 1 - stopWatch_state;
    }
            break;
        case BUTTON_RELEASED:
            if(!readButtonRB0()){
                buttonStateRB0 = BUTTON_PRESSED;
            }
            break;
        default:
            break;
    }
}
void FSM(){
   // LATD = state;
    switch (state){
        case 0: fBEGIN();   
            break;
        case 1: fCLOCK();
            break;
        case 2: fHOUR();
            break;
        case 3: fMINUTE();
            break;
        case 4: fSECOND();
            break;
        case 5: fSW(); 
           break;
    }
   // if (access_state) count_Clock();
}
void main(void) {
    SYSTEM_Initialize();

    INTERRUPT_GlobalInterruptEnable();

    INTERRUPT_PeripheralInterruptEnable();

   
    while (1){
        FSM();
        buttonFSMRA5();
        buttonFSMRB0();
    }
    return;
}
