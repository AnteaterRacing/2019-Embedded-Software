#include <msp430g2553.h>
#include "PWM.h"


void init_PWM(){
    P2DIR |= BIT1;
    P2SEL |= BIT1;
    P1DIR |= BIT6;
    P1SEL |= BIT6;

    TA1CCR0 |= 625;
    TA1CCTL1 |= OUTMOD_7;



    TA0CCR0 = 625;
    TA0CCTL1 = OUTMOD_7;




}
