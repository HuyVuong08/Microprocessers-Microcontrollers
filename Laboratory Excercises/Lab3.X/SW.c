#include "SW.h"
#include "Button.h"

void disp_SW() {
    mCURSOR_LINE1;
    LCDPutStr("    ");
    LCDPutChar(sw_minute / 10 + '0');
    LCDPutChar(sw_minute % 10 + '0');
    LCDPutChar(':');
    LCDPutChar(sw_second / 10 + '0');
    LCDPutChar(sw_second % 10 + '0');
    LCDPutChar(':');
    LCDPutChar(sw_10ms / 10 + '0');
    LCDPutChar(sw_10ms % 10 + '0');
    LCDPutStr("    ");
    mCURSOR_LINE2;
    LCDPutStr("RB0=Ctr RA5=Next");
    if (stopWatch_state) start_stopWatch();
}

void start_stopWatch() {
    if (timerflag == 1) 
    {
        sw_10ms++; 
        if (sw_10ms > 99) 
        {
            sw_10ms = 0; 
            sw_second++; 
        }
        if (sw_second > 59) 
        {
            sw_second = 0; 
            sw_minute++; 
        }
        if (sw_minute > 59) 
        {
            sw_minute = 0; 
        }
        timerflag = 0;
    }
}

void count_Clock_SW() {
    if (swflag == 1) 
    {
        count10ms++;
        if (count10ms > 99) {
            count10ms = 0;
            second++;
        }
        if (second > 59) 
        {
            second = 0; 
            minute++; 
        }
        if (minute > 59) 
        {
            minute = 0; 
            hour++; 
        }
        if (hour > 23) 
        {
            hour = 0; 
        }
        swflag = 0;
    }
}

void fSW() {

    if (checkSw) {

        mCURSOR_LINE1;
        LCDPutStr("     STOPWATCH      ");
        mCURSOR_LINE2;
        LCDPutStr("RB0=Now RA5=Next"); 
        checkClock = 1;
        checkHr = 1;
        checkMin = 1;
        checkSc = 1;
        checkSw = 0;
    }
    if (access_state) {
        if (firstTime) {
            stopWatch_state = 0;
            firstTime = 0;
        }
        disp_SW();
        count_Clock_SW();
    }
}
