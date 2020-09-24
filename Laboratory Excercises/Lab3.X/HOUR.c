#include "HOUR.h"
#include "Button.h"
static int count = 0;
static int blinkflag = 0;
void disp_HOUR() 
{
    if (timerflag)
    {
        count++;
        timerflag = 0;
    }
    if (count >= 50){
        blinkflag = 1 - blinkflag;
        count = 0;
    } 
    mCURSOR_LINE1;
    LCDPutStr("    ");
    if (blinkflag)
    {    
        LCDPutChar(hour/10+'0');
        LCDPutChar(hour%10+'0');
    }
    else 
    {
        LCDPutStr("  ");
    }
    LCDPutChar(':');
    LCDPutChar(minute/10+'0');
    LCDPutChar(minute%10+'0');
    LCDPutChar(':');
    LCDPutChar(second/10+'0');
    LCDPutChar(second%10+'0');
    LCDPutStr("    ");
    mCURSOR_LINE2;
    LCDPutStr("        RA5=Next");
    modify_HOUR();
}
void modify_HOUR(void)
{
    if ((pRB0 && !normalFlag) || FastIncreaseFlag){
        hour = (hour + 1)%24;
        normalFlag = 1;
        FastIncreaseFlag = 0;
    }
}
void fHOUR()
{
    if (checkHr)             
        {
            mCURSOR_LINE1;
            LCDPutStr("     HOUR      ");
            mCURSOR_LINE2;
            LCDPutStr("RB0=Now RA5=Next");
            checkClock = 1; 
            checkHr = 0;
            checkMin = 1;
            checkSc = 1;
            checkSw = 1;
        }
    if (access_state == 1)
    {
        disp_HOUR();
    }
}
