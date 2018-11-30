

int adc[4];
char acc1Input;
char acc2Input;
char brakeInput;
char steeringInput;
char * adcCHAR;

__interrupt void ADC10_ISR(void);

void init_ADC();

