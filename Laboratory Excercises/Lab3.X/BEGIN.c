#include "BEGIN.h"
#include "Button.h"

void disp_BEGIN() {
    if (beginFlag == 0) {
        mOPEN_LCD;
        beginFlag = 1;
        mCURSOR_LINE1;
        LCDPutStr("Microchip PICDEM");
        mCURSOR_LINE2;
        LCDPutStr(" PIC18 Explorer ");
        BEGIN_delay();
        state = 1;
 
    }

}
void BEGIN_delay (void)
{
    unsigned char i;
    for (i=0;i<25;i++)
    {__delay_ms(50);}
}
void fBEGIN() {
    disp_BEGIN();
}

