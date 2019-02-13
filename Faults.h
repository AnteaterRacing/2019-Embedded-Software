/*
Component: Driver Input Module
Engineer: Lucas Juttner
Company: UC Irvine Anteater Electric Racing
Date: February 12, 2019
Description: The Driver Input Module (DIM) is a Texas Instruments MSP430G2ET (implemented with a G2553 integrated circuit) 
micro-controller with the requirements of handling driver input of Lithium, UC Irvine’s 2019 FSAE Electric Racecar 
competing in Lincoln, Nebraska. 
*/


//returns 1 if fault, 0 if no fault. (checks acc pedal transfer functions)
int APPS_Fault(int,int);

//returns 1 if BSE fault, 0 if no fault (checks that acc is not depressed when brake is depressed >10%)
int BSE_Fault(int,int,int);
