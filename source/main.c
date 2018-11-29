#include <msp430g2553.h>
#include "Faults.h"
#include "PWM.h"
#include "ADC.h"
#include "UART.h"
#include <stdint.h>
/**
 * main.c
 */
int AcceleratorL;  //Left motor duty cycle
int AcceleratorR;  //Right motor duty cycle
int steeringval;
int accval;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    BCSCTL1 = CALBC1_1MHZ; // Set range
    DCOCTL = CALDCO_1MHZ;
    init_ADC();
    init_UART();
    __enable_interrupt();

    while (1) {

        ADC10CTL0 &= ~ENC;         // Sampling and conversion start
        while (ADC10CTL1 & BUSY);
        ADC10SA = (unsigned int)adc;
   //     ADC10CTL0 |= ENC + ADC10SC;

        test_UART(0);
        test_UART(1);
        test_UART(2);
        test_UART(3);
    }

//        ADC_Values();
//        AcceleratorL = acc1Input;
//        AcceleratorR = (1024 - acc2Input);
//        setThrottleValue(AcceleratorL,AcceleratorR);
//        if(APPS_Fault(acc1Input,acc2Input) || BSE_Fault(brakeInput,acc1Input,acc2Input)){
//            AcceleratorL = 0;
//            AcceleratorR = 0;
//            setThrottleValue(AcceleratorL,AcceleratorR);
//        }
//        else {
//              setThrottleValue(AcceleratorL,AcceleratorR);
//            float B = .4; // offset of curve for torque vectoring
//            float A = .6/(72); // slope of curve for left  turn torque vectoring
//            float D = .6/(77); // slope of curve for right turn torque vectoring
//            if (acc1Input >= 127){
//                accval = 255;
//            }
//            else if(acc1Input > 50) {
//                accval = 2*acc1Input;
//            }
//            else {
//                accval = 0;
//            }
//            steeringval = steeringInput; //steering potentiometer value
//            //TORQUE VECTORING BASIC ALGORITHM
//            if (steeringval < 93) { //left turn calibrated for 20% margine b4 turn engaging
//                AcceleratorR = accval *( A*(steeringval - 21) + B); //calibrated so it is 40% at full turn
//                AcceleratorL = accval;
//
//            } else if (steeringval > 133) {                 //right turn calibrated for 20% margine
//                AcceleratorR = accval;
//                AcceleratorL = accval *(D *(210-steeringval) + B);  //calibrated so it is 40% at full turn
//
//            } else { //on center steering. deadzone between 108 and 148
//                AcceleratorR = accval;
//                AcceleratorL = accval;
//            }

        //}
    }


