#include "CLOCK.h"
#include "Button.h"

void disp_CLOCK()
{
    mCURSOR_LINE1;
    LCDPutStr("    ");
    LCDPutChar(hour/10+'0');
    LCDPutChar(hour%10+'0');
    LCDPutChar(':');
    LCDPutChar(minute/10+'0');
    LCDPutChar(minute%10+'0');
    LCDPutChar(':');
    LCDPutChar(second/10+'0');
    LCDPutChar(second%10+'0');
    LCDPutStr("    ");
}

void fCLOCK()
{
    if (checkClock)             
    {
        firstTime =1 ;
        sw_10ms = 0;
        sw_second = 0;
        sw_minute = 0;
        mCURSOR_LINE1;
        LCDPutStr("     CLOCK      ");
        mCURSOR_LINE2;
        LCDPutStr("RB0=Now RA5=Next");
        checkClock = 0; 
        checkHr = 1;
        checkMin = 1;
        checkSc = 1;
        checkSw = 1;
    }
        
    if (access_state)             
    {
        count_Clock ();                 
    }
}

void count_Clock (void)
{
    if(timerflag == 1)             
    {
        count10ms++;
        if (count10ms > 99)
        {
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
        disp_CLOCK();                   
        timerflag = 0;    

        mCURSOR_LINE2;
        LCDPutStr("        RA5=Next");      
    }
//    }
}
