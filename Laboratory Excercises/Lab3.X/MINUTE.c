#include "MINUTE.h"
#include "Button.h"
static int count = 0;
static int blinkflag = 0;
void disp_MINUTE() {
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
    LCDPutChar(hour/10+'0');
    LCDPutChar(hour%10+'0');
    LCDPutChar(':');
    if (blinkflag)
    {
    
        LCDPutChar(minute/10+'0');
        LCDPutChar(minute%10+'0');
    }
    else 
    {
        LCDPutStr("  ");
    }
    LCDPutChar(':');
    LCDPutChar(second/10+'0');
    LCDPutChar(second%10+'0');
    LCDPutStr("    ");
    mCURSOR_LINE2;
    LCDPutStr("        RA5=Next");
    modify_MIN();
}
void modify_MIN(void){
    if ((pRB0 && !normalFlag) || FastIncreaseFlag){
        minute = (minute + 1)%60;
        normalFlag = 1;
        FastIncreaseFlag = 0;
    }
}
void fMINUTE() {
    if (checkMin)             
        {
             
            mCURSOR_LINE1;
            LCDPutStr("     MINUTE      ");
            mCURSOR_LINE2;
            LCDPutStr("RB0=Now RA5=Next");
            checkClock = 1; 
            checkHr = 1;
            checkMin = 0;
            checkSc = 1;
            checkSw = 1;
        }
    if (access_state == 1)
    {
        disp_MINUTE();
    }
}
