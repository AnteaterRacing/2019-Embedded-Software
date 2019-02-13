/*
Component: Driver Input Module
Engineer: Lucas Juttner
Company: UC Irvine Anteater Electric Racing
Date: February 12, 2019
Description: The Driver Input Module (DIM) is a Texas Instruments MSP430G2ET (implemented with a G2553 integrated circuit) 
micro-controller with the requirements of handling driver input of Lithium, UC Irvine’s 2019 FSAE Electric Racecar 
competing in Lincoln, Nebraska. 
*/

unsigned int adc[3];
int acc1Input;
int acc2Input;
int brakeInput;
int steeringInput;


__interrupt void ADC10_ISR(void);

void init_ADC();
void read_ADC();
