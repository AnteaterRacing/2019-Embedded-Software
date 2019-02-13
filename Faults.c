/*
Component: Driver Input Module
Engineer: Lucas Juttner
Company: UC Irvine Anteater Electric Racing
Date: February 12, 2019
Description: The Driver Input Module (DIM) is a Texas Instruments MSP430G2ET (implemented with a G2553 integrated circuit) 
micro-controller with the requirements of handling driver input of Lithium, UC Irvine’s 2019 FSAE Electric Racecar 
competing in Lincoln, Nebraska. 
*/

#include <Faults.h>
#include <stdint.h>

int BSE_flag = 0;
//returns 1 if fault, 0 if no fault. (checks acc pedal transfer functions)
int APPS_Fault(int acc1, int acc2){
    //102 is 10% of 1023 which is the max value for ADC inputs
    //if acc1 or 2 is <10 then there is a disconnect on the line since it is pulled down.
    //if apps flag has been already triggered but fault is still occurring, do nothing
    acc2 = 1023 - (acc2*4.5);		//Accelerator 2 potentiometer has a negative transfer function to Accelerator 1
    if((abs(acc1-acc2) > 102) || (acc1 < 10) || (acc2 < 10)) {
        return 1;
    }
    //if there is no fault occurring, disable APPS flag if it is triggered and continue execution.
    else {
        return 0;
    }
}

//returns 1 if BSE fault, 0 if no fault (checks that brake is not depressed when acc is depressed >25%)
int BSE_Fault(int brakeAngle, int acc1, int acc2){
    acc2 = 1023 - (acc2*4.5);		//Accelerator 2 potentiometer has a negative transfer function to Accelerator 1
    if((BSE_flag) && (acc1 > 51 || acc2 > 51)) { //51 is 5% of 1023
        return 1;
    }
    if((acc1 > 256 || acc2 > 256) && brakeAngle > 542){ //256 is 25% of 1023      542 is 53% of 5V where 2.67V is "hard press" of brakes
        BSE_flag = 1;
        return 1;
    }
    else if ((BSE_flag) && (acc1 < 51 && acc2 < 51)){ //51 is 5% of 1023  if flag is already set, flag will clear when APPS values are less than 5%
        BSE_flag = 0;
        return 0;
    }
    return 0;

}
