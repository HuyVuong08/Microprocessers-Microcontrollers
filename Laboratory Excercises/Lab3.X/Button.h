#ifndef BUTTONS_H
#define	BUTTONS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "mcc.h"
#include "LCD.h"

int firstReadRA5;
int secondReadRA5;
int thirdReadRA5;
int firstReadRB0;
int secondReadRB0;
int thirdReadRB0;
int readButtonRA5();
int readButtonRB0();
// Configuration bits

int pRB0 = 0;
int countpTime = 0;
int countMode = 0;
//int fastMode = 0;
int hour = 0;
int minute = 0;
int second = 0;
int count10ms = 0;

int sw_10ms = 0;
int sw_second = 0;
int sw_minute = 0;
int access_state = 0;

int clock_state = 0;

int stopWatch_state = 0;
int sw_preVal = 0;

unsigned char FastIncreaseFlag = 0;
unsigned char secondFlag = 0;
unsigned char secondSWFlag = 0;
unsigned char stopWatch_flag = 1;
unsigned char fastMode = 0;
unsigned char normalFlag = 0; 
int beginFlag = 0;
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



// Definitions for select and scroll buttons
#define DfIbutton TRISAbits.TRISA5
#define Ibutton PORTAbits.RA5
#define DfDbutton  TRISBbits.TRISB0
#define Dbutton PORTBbits.RB0
#define press 0
#define FASTTIME 50
#define FASTMODE 100
#define BUTTON_PRESSED 0
#define BUTTON_RELEASED 1
unsigned char buttonStateRA5 = 0;
unsigned char buttonStateRB0 = 0;
void buttonFSMRB0();
int checkBegin = 1;
int checkClock = 1;
int checkHr = 1;
int checkMin = 1;
int checkSc = 1;
int checkSw = 1;
int firstTime = 1;
void fBEGIN (void);

void fCLOCK (void);

void fHOUR (void);

void fMINUTE (void);

void fSECOND (void);

void fSW(void);
// Enumerations
#define BEGIN 0
#define CLOCK 1
#define CHANGE_HOUR 2
#define CHANGE_MINUTE 3
#define CHANGE_SECOND 4
#define STOPWATCH 5
    


unsigned int state = BEGIN;

#ifdef	__cplusplus
}
#endif

#endif	/* BUTTONS_H */

