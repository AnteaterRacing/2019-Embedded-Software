//##########################################################################################################################################################
// Anteater Electric Racing  "Driver Input Module"
//
// Engineer: Lucas Juttner
// Date: 4/23/2019
// School: University of California, Irvine
// File: Faults.c
//
// Description: Function checks fault conditions for accelrator pedal position sensors
//
//##########################################################################################################################################################

#include <Faults.h>
#include <stdint.h>

//----------------------------------------------- Function to check Faults ---------------------------------------------------------------------------------

int APPS_Fault(int acc1, int acc2){
    acc2 = 1023 - (acc2*4.5);
    if((abs(acc1-acc2) > 102) || (acc1 < 10) || (acc2 < 10)) {      //Checks for fault
        return 1;                                                   //If fault occurs, return 1
    }
    else {
        return 0;                                                   //If not fault, return 0
    }
}
//102 is 10% of 1023 which is the max value for ADC inputs
//if acc1 or 2 is <10 then there is a disconnect on the line since it is pulled down.
//if apps flag has been already triggered but fault is still occurring, do nothing
