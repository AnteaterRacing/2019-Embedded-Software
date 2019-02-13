/*
Component: Driver Input Module
Engineer: Lucas Juttner
Company: UC Irvine Anteater Electric Racing
Date: February 12, 2019
Description: The Driver Input Module (DIM) is a Texas Instruments MSP430G2ET (implemented with a G2553 integrated circuit) 
micro-controller with the requirements of handling driver input of Lithium, UC Irvine’s 2019 FSAE Electric Racecar 
competing in Lincoln, Nebraska. 
*/

int AcceleratorL;  //Left motor duty cycle
int AcceleratorR;  //Right motor duty cycle

__interrupt void Timer_A0_CC0(void);
__interrupt void Timer_A1_CC1(void);

void init_PWM();

void setThrottleValue(int , int );
