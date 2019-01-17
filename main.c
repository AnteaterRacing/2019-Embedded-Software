#include <ADC.h>
#include <Faults.h>
#include <msp430g2553.h>
#include <PWM.h>
#include <SPI.h>
#include <stdint.h>
#include <UART.h>

/**
 * main.c
 */

int steeringval;
int accval;
int acc2New;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    BCSCTL1 = CALBC1_16MHZ; // Set range
    DCOCTL = CALDCO_16MHZ;
    init_ADC();
//   init_UART();
    init_PWM();
  //  __enable_interrupt();

    while (1) {
        read_ADC();
     //   int spi_data[4] = {acc1Input, acc2Input, brakeInput, steeringInput};
    //    transmit_SPI(spi_data);
        acc2New = 1023 - acc2Input;
        AcceleratorL = acc1Input*0.605; //0.611 is conversion factor from digital value to duty cycle value
        AcceleratorR = acc1Input*0.605;
//        setThrottleValue(AcceleratorL,AcceleratorR);
        if(APPS_Fault(acc1Input,acc2Input) || BSE_Fault(brakeInput,acc1Input,acc2Input)){
            AcceleratorL = 0;
            AcceleratorR = 0;
            setThrottleValue(AcceleratorL,AcceleratorR);
        }
//        else{
//            setThrottleValue(AcceleratorL,AcceleratorR);
//        }
        else {
            float B = 0.4; // offset of curve for torque vectoring
            float A = 0.6/(72); // slope of curve for left  turn torque vectoring
            float D = 0.6/(77); // slope of curve for right turn torque vectoring
            if (acc1Input >= 512){
                AcceleratorL = 1023*0.605;
                AcceleratorR = 1023*0.605;
            }
            else if(acc1Input > 201) {
                AcceleratorL = 2*acc1Input*0.605;
                AcceleratorR = 2*acc1Input*0.605;
            }
            else {
                AcceleratorL = 0;
                AcceleratorR = 0;
            }
            steeringval = steeringInput; //steering potentiometer value
            //TORQUE VECTORING BASIC ALGORITHM
            if (steeringval < 373 && steeringval > 84) { //left turn calibrated for 20% margine b4 turn engaging
                AcceleratorR = acc1Input *( A*((512-steeringval) - 84) + B)*0.605; //calibrated so it is 40% at full turn
                AcceleratorL = acc1Input*0.605;
            }
            else if (steeringval < 84){
                AcceleratorR = 1023*0.605;
                AcceleratorL = acc1Input*0.605;
            }
            else if (steeringval > 534 && steeringval < 842) {                 //right turn calibrated for 20% margine
                AcceleratorR = acc1Input*0.605;
                AcceleratorL = acc1Input *(D *(842-(1023-steeringval)) + B)*0.605;  //calibrated so it is 40% at full turn

            }
            else if (steeringval > 842){
                AcceleratorR = acc1Input*0.605;
                AcceleratorL = 1023*0.605;
            }
            else { //on center steering.
                AcceleratorR = acc1Input*0.605;
                AcceleratorL = acc1Input*0.605;
            }

            setThrottleValue(AcceleratorL,AcceleratorR);
        }


    }


//        AcceleratorL = acc1Input;
//        AcceleratorR = (1023 - acc2Input);
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


