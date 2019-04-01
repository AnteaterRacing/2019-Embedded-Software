

unsigned int adc[6];
int acc1Input;
int acc2Input;
int brakeInput;
int steeringInput;


__interrupt void ADC10_ISR(void);

void init_ADC();
void read_ADC();
