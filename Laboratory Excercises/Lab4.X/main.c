/*
 * File:   main.c
 * Author: ngqui
 *
 * Created on October 7, 2019, 12:28 PM
 */
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

struct callback_member{
    int time ;
    unsigned char timer_flag ;
    void (*callback_function)(void);
    void (*function_data)(void);
    int mem_counter; 
    unsigned char run_flag;
};


typedef unsigned long int timestamp_t;
typedef void timer_callback_t;
//typedef unsigned long int uint64_t;


#define INTERNAL_CLOCK 8000000
#define MAX_CALLBACK 10


long long int timer_counter;
int timer_var = 10;
int index = 0;
struct callback_member list[MAX_CALLBACK];

int hash(int index);
void set_timer();
void TMR1_Start(void);
void TMR1_Stop(void);

int start_timer(int timer_vaddr);
timestamp_t get_time(void);
uint32_t register_timer(uint64_t delay, timer_callback_t (* callback)(), void *data ); 
int remove_timer(uint32_t id);
int stop_timer(void);
int timer_ISR();    


void __interrupt() process(void);

void OSCILLATOR(void) ;
void PIN_MANAGER(void) ;
void INTERRUPT() ;
void ADC(void) ;
void TMR1_IN(void) ;

void SYSTEM_IN() ;
void STRUC_IN();
void increaseLED(){
    LATD++;
}
void setLED(){
    LATD = 0x0F;
}
void main(void) {
    int loc;
    SYSTEM_IN();
    //LATD = 0x06;
   // start_timer(10);
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    register_timer(100, increaseLED, NULL);
    register_timer(300, setLED, NULL);
    while (1) {
        for (int i = 0; i < MAX_CALLBACK; i++){
            if (list[i].run_flag) {
                (*list[i].callback_function)();
                list[i].run_flag = 0;
            }
        }
    }
    return;
}

void OSCILLATOR(void) {
    OSCCON = 0x76; //Set internal clock = 8MHz/4 
    OSCTUNE = 0x00;
}

void PIN_MANAGER(void) {
//    DfIbutton = 1;
//    DfDbutton = 1;
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

void TMR1_IN(void) {
    T1CON = 0x7D; //Set pre-scale = 8, enable internal clock = 2MHz / 8 = 250kHz
    /*
     Normally, Timer1 count from 0 to FFFF,
     * Assume that we need a clock of x (s), then we need f = x.internal_clock cycles before interrupt
     * So {TMR1H:TMR1L} = FFFF - f
     * 
     */
    set_timer();
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 1;
    TMR1_Start();
}

void STRUCT_IN(){
    for (int i = 0; i < MAX_CALLBACK; i++){
        list[i].mem_counter = 0;
        list[i].run_flag = 0;
        list[i].time = 0;
        list[i].timer_flag = 0;
    }
}
void SYSTEM_IN() {
    OSCILLATOR();
    PIN_MANAGER();
    ADC();
    INTERRUPT();
    TMR1_IN();
    STRUCT_IN();
}

void set_timer(){
    unsigned long int frequency = INTERNAL_CLOCK/32;
    int step = 1000/timer_var;
    int unit = frequency/step;
    TMR1L = 0xFFFF - unit;
   // LATD = frequency;
    TMR1H = (0xFFFF - unit) >> 8;
    
}
void TMR1_Start(void) {
    T1CONbits.TMR1ON = 1;
}

void TMR1_Stop(void) {
    T1CONbits.TMR1ON = 0;
}

int hash(int index){
    int result;
    result = index % MAX_CALLBACK;
    while (list[result].timer_flag)
        result = (result + 1) % MAX_CALLBACK;
    return result;
}

int start_timer(int timer_vaddr){
    timer_var = timer_vaddr;
    timer_counter = 0;
}
timestamp_t get_time(void){
    return timer_counter*timer_var;
}
int stop_timer(void){
    TMR1_Stop();
}

int timer_ISR(){
    set_timer();
    PIR1bits.TMR1IF = 0 ;
    //LATD++;
    timer_counter++;
    for (int i = 0; i < index; i++)
    {
        if (list[i].timer_flag && list[i].timer_flag)
        {
            list[i].mem_counter--;
            //LATD = list[i].time;
            if (list[i].mem_counter <= 0)
            {
                //LATD++;
                list[i].run_flag = 1;
                list[i].mem_counter = list[i].time;
            }
        }
    }
    
}

uint32_t register_timer(uint64_t delay, timer_callback_t (* callback)(), void *data ){
    int loc = hash(index);
    //LATD = loc + 2;
    list[loc].time = delay;
    list[loc].timer_flag = 1;
    list[loc].callback_function = callback;
    list[loc].function_data = data;
    list[loc].run_flag = delay;
    index++;
}

int remove_timer(uint32_t id){
    int loc = hash(id);
    list[loc].time = 0;
    list[loc].timer_flag = 0;
    list[loc].callback_function = 0;
    list[loc].function_data = 0;
    list[loc].run_flag = 0;
    //index++;
}

void __interrupt() process(void) {
    if (PIE1bits.TMR1IE == 1 && PIR1bits.TMR1IF == 1) {
        timer_ISR();
        //set_timer();
    } else {

    }
}