//##########################################################################################################################################################
// Anteater Electric Racing  "Driver Input Module"
//
// Engineer: Lucas Juttner
// Date: 4/23/2019
// School: University of California, Irvine
// File: ADC.h
//
// Description: Functions initialize Analog to Digital Converter and place input values into adc buffer
//
//##########################################################################################################################################################


unsigned int adc[4];
int acc1Input;
int acc2Input;
int brakeInput;
int steeringInput;

void init_ADC();
void read_ADC();
__interrupt void ADC10_ISR(void);

