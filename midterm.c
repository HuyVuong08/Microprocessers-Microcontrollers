//Write a program to control water pump.

//Q1: FSM consists of two stages: Check sensor <-> Control water pump

//Q2:

//Define port
#define DfSensor TRISAbits.TRISA5       
#define Sensor PORTAbits.RA5            //Sensor is connect to RA5 bit of portA
#define DfPump  TRISBbits.TRISB1
#define Pump PORTBbits.RB1           //Water pump is connect to RB1 bit of portB
#define HighWaterLvDetected 0
//Define FSM
#define SensorCheck 0
#define PumpControl 1

unsigned int state = SensorCheck;
//Define variable for read Sensor
int HighWaterLv = 0;
//Reload the timer
volatile uint16_t timerReloadVal;
//Checking the stable of Sensor
int firstRead;
int secondRead;

void OSCILLATOR(void) {
    OSCCON = 0x76; //Set internal clock = 8MHz/4 
    OSCTUNE = 0x00;
}

void PIN_MANAGER(void) {
    DfSensor = 1;           //Modify RA5 bit of portA to input mode
    DfPump = 0;             //Modify RB1 bit of portB to input mode
    Pump = 1;               //Pump is initially switched on 
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
    /* Delay 10ms
     * RegValue = 65536-(Delay * Fosc)/(Prescalar*4))
     * Normally, Timer1 count from 0 to FFFF,
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

int readSensor() {
    firstRead = secondRead;
    secondRead = PORTAbits.RA5;
    if (firstReadRB0 == secondReadRB0)
        if (firstReadRB0 == HighWaterLvDetected) return 1;
    return 0;
}

void TMR1_ISR() {
    PIR1bits.TMR1IF = 0;
    TMR1_Reload();
    if (readSensorRA5()) {
        HighWaterLv = 1;
    } else {
        HighWaterLv = 0;
    }
}

void __interrupt() CheckButtonsPress() {
    if (PIE1bits.TMR1IE == 1 && PIR1bits.TMR1IF == 1) {
        TMR1_ISR();
    } else {

    }
}

void FSM() {
    switch (state) {
        case SensorCheck:
            if (HighWaterLv) {
                state = PumpControl;
            } else if (!HighWaterLv) {
                LATBbits.LATB1 = 1;
            }
            break;
        case PumpControl:
            LATBbits.LATB1 = 0;
            state = SensorCheck;
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
