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
#include <Faults.h>
#include <PWM.h>
#include <msp430g2553.h>
#include <stdint.h>


int steeringval;
int accval;
int acc2New;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    BCSCTL1 = CALBC1_16MHZ; // Set range
    DCOCTL = CALDCO_16MHZ;
    init_ADC();		//initiate ADC 
    init_PWM();		//initiate PWM
    while (1) {
		read_ADC();		//read input from potentiometers
		//make sure to calibrate acc2Input either here, or in both the APPS and BSE fault functions
        if(APPS_Fault(acc1Input,acc2Input) || BSE_Fault(brakeInput,acc1Input,acc2Input)){
           AcceleratorL = 0;		//Set throttle signals to 0 if fault occurs
           AcceleratorR = 0;
           setThrottleValue(AcceleratorL,AcceleratorR);
        }
        else {
			int accel = acc1Input;
			float B = 0.4; // offset of curve for torque vectoring
			float A = 0.6/(72); // slope of curve for left  turn torque vectoring
			float D = 0.6/(77); // slope of curve for right turn torque vectoring
			steeringval = steeringInput; //steering potentiometer value
			//TORQUE VECTORING BASIC ALGORITHM
			if (steeringval < 373 && steeringval > 84) { //left turn calibrated for 20% margine b4 turn engaging
				AcceleratorR = accel *( A*((512-steeringval) - 84) + B)*0.605; //calibrated so it is 40% at full turn
				AcceleratorL = accel*0.605;
            }
			else if (steeringval < 84){
				AcceleratorR = 1023*0.605;
				AcceleratorL = accel*0.605;
			}
			else if (steeringval > 534 && steeringval < 842) {                 //right turn calibrated for 20% margine
				AcceleratorR = accel*0.605;
				AcceleratorL = accel *(D *(842-(1023-steeringval)) + B)*0.605;  //calibrated so it is 40% at full turn

			}
			else if (steeringval > 842){
				AcceleratorR = accel*0.605;
				AcceleratorL = 1023*0.605;
			}
			else { //on center steering.
				AcceleratorR = accel*0.605;
				AcceleratorL = accel*0.605;
			}

			setThrottleValue(AcceleratorL,AcceleratorR);		//Sends accelerator signals for PWM output
		}
    }
}
