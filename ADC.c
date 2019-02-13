/*
Component: Driver Input Module
Engineer: Lucas Juttner
Company: UC Irvine Anteater Electric Racing
Date: February 12, 2019
Description: The Driver Input Module (DIM) is a Texas Instruments MSP430G2ET (implemented with a G2553 integrated circuit) 
micro-controller with the requirements of handling driver input of Lithium, UC Irvine’s 2019 FSAE Electric Racecar 
competing in Lincoln, Nebraska. 
*/

#include <ADC.h>
#include <msp430g2553.h>

// ADC10 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR (void)
#else
#error Compiler not supported!
#endif
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}

void init_ADC(){
    ADC10CTL1 = INCH_3 + CONSEQ_1; //Channel 3 down to 0  and sets up single channel conversion
    ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
    ADC10DTC1 = 4;
    ADC10AE0 = BIT3 + BIT2 + BIT1 + BIT0;   //enables analog on pin 1.0, 1.1, 1.2, and 1.3
}

void read_ADC(){
    ADC10CTL0 &= ~ENC;
    while(ADC10CTL1 & BUSY);
    ADC10SA = (unsigned int)adc;
    ADC10CTL0 |= ENC + ADC10SC;
    __bis_SR_register(CPUOFF + GIE);

    steeringInput = adc[0];
    brakeInput = adc[1];
    acc1Input = adc[2];
    acc2Input = adc[3];
}
