#include "Button.h"
int readButtonRB0(){
    firstReadRB0 = secondReadRB0;
    secondReadRB0 = thirdReadRB0;
    thirdReadRB0 = PORTBbits.RB0;
    if (firstReadRB0 == thirdReadRB0 )
        if (firstReadRB0 == 0) return 1;
    return 0; 
}
int readButtonRA5(){
    firstReadRA5 = secondReadRA5;
    secondReadRA5 = thirdReadRA5;
    thirdReadRA5 = PORTAbits.RA5;
    if (firstReadRA5 == thirdReadRA5)
        if (firstReadRA5 == 0) return 1;
    return 0;
}


