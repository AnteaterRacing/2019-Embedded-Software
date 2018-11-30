#include <Faults.h>
#include <stdint.h>
//#include "main.h"
int BSE_flag = 0;
//returns 1 if fault, 0 if no fault. (checks acc pedal transfer functions)
int APPS_Fault(int acc1, int acc2){
    //25 is 10% of 255 which is the max value for ADC inputs
    //if acc1 or 2 is <10 then there is a disconnect on the line since it is pulled down.
    //if apps flag has been already triggered but fault is still occurring, do nothing
    acc2 = 1023 - acc2;
    if((abs(acc1-acc2) > 25) || (acc1 < 10) || (acc2 < 10)) {
        return 1;
    }
    //if there is no fault occurring, disable APPS flag if it is triggered and continue execution.
    else {
        return 0;
    }
}

//returns 1 if APBS fault, 0 if no fault (checks that brake is not depressed when acc is depressed >25%)
int BSE_Fault(int brakeAngle, int acc1, int acc2){
    acc2 = 1023 - acc2;
    if((BSE_flag) && (acc1 > 13 || acc2 > 13)) { //13 is 5% of 255
        return 1;
    }
    if((acc1 > 64 || acc2 > 64) && brakeAngle > 0x88){ //64 is 25% of 255      0x88 is 53% of 5V where 2.67V is "hard press" of brakes
        BSE_flag = 1;
        return 1;
    }
    else if ((BSE_flag) && (acc1 < 13 && acc2 < 13)){ //13 is 5% of 255  if flag is already set, flag will clear when APPS values are less than 5%
        BSE_flag = 0;
        return 0;
    }
    return 0;

}
