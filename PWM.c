/*
Component: Driver Input Module
Engineer: Lucas Juttner
Company: UC Irvine Anteater Electric Racing
Date: February 12, 2019
Description: The Driver Input Module (DIM) is a Texas Instruments MSP430G2ET (implemented with a G2553 integrated circuit) 
micro-controller with the requirements of handling driver input of Lithium, UC Irvines 2019 FSAE Electric Racecar
competing in Lincoln, Nebraska. 
*/

#include <msp430g2553.h>
#include "PWM.h"

void init_PWM(){
    P2DIR |= BIT1;
    P2SEL |= BIT1;
    P1DIR |= BIT6;
    P1SEL |= BIT6;

    TA1CCR0 |= 624;  //625 is the period for 1.6kHz so value is 0-624
    TA1CCTL1 |= OUTMOD_7;
    TA1CCR1 |= 0;   //initialize PWM at 0 duty cycle
    TA1CTL |= TASSEL_2 + MC_1;

    TA0CCR0 = 624;	//625 is the period for 1.6kHz so value is 0-624
    TA0CCTL1 = OUTMOD_7;
    TA0CCR1 |= 0;	//initialize PWM at 0 duty cycle
    TA0CTL |= TASSEL_2 + MC_1;
}

void setThrottleValue(int accL, int accR){
//    if(accL < 5){
//        accL = 5;
//    }
//    else if(accL > 1015){
//        accL = 1015;
//    }
//    if(accR < 5){
//        accR = 5;
//    }
//    else if(accR > 1015){
//        accR = 1015;
//    }
    TA1CCR1 = accL;
    TA0CCR1 = accR;
  //      __bis_SR_register(LPM0_bits);
}
