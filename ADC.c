#include <ADC.h>
#include <msp430g2553.h>

__interrupt void ADC10_ISR(void){
__bic_SR_register_on_exit(CPUOFF);
}
void init_ADC(){
    ADC10CTL1 = INCH_5 + ADC10DIV_0 + CONSEQ_3 + SHS_0; //Channel 5 down to 0  and sets up multi channel conversion
    ADC10CTL0 = SREF_0 + ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
    ADC10AE0 = BIT5 + BIT4 + BIT3 + BIT0;   //enables analog on pin 1.0, 1.3, 1.4, and 1.5
    ADC10DTC1 = 0x04;
}
