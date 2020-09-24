#include "SECOND.h"
#include "Button.h"
static int count = 0;
static int blinkflag = 0;
void disp_SECOND() {
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
    LCDPutChar(minute/10+'0');
    LCDPutChar(minute%10+'0');
    LCDPutChar(':');
      if (blinkflag)
    {
    
        LCDPutChar(second/10+'0');
        LCDPutChar(second%10+'0');
    }
    else 
    {
        LCDPutStr("  ");
    }
    LCDPutStr("    ");
    mCURSOR_LINE2;
    LCDPutStr("        RA5=Next");
    modify_SEC();
}
void modify_SEC(){
     if ((pRB0 && !normalFlag) || FastIncreaseFlag){
        second = (second + 1)%60;
        normalFlag = 1;
        FastIncreaseFlag = 0;
    }
}
void fSECOND() {
    if (checkSc)             
        {
            
            mCURSOR_LINE1;
            LCDPutStr("     SECOND      ");
            mCURSOR_LINE2;
            LCDPutStr("RB0=Now RA5=Next");// Switch to Voltmeter application
            checkClock = 1; 
            checkHr = 1;
            checkMin = 1;
            checkSc = 0;
            checkSw = 1;
        }
    if (access_state == 1)
    {
        disp_SECOND();
    }
}
