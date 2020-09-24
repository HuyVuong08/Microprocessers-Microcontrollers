
#ifndef STATE_CLOCK_H
#define	STATE_CLOCK_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "Button.h"

//volatile unsigned char hour = 0, minute = 0, second = 0, count10ms = 0;

// Enumerations

void disp_Clock (void);

void count_Clock (void);

#ifdef	__cplusplus
}
#endif

#endif	/* STATE_CLOCK_H */