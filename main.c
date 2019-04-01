#include <ADC.h>
#include <Faults.h>
#include <msp430g2553.h>
#include <PWM.h>
#include <SPI.h>
#include <stdint.h>
#include <UART.h>
#include <mcp2515.h>

/**
 * main.c
 */

int steeringval;
int accval;
int acc2New;
can_t can_tx;
can_t can_rx;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    BCSCTL1 = CALBC1_8MHZ; // Set range
    DCOCTL = CALDCO_8MHZ;
  //  init_ADC();
//   init_UART();
    //init_PWM();
    P1DIR |= BIT0;                                                                 // P1.0 als Ausgang (LED am EXP430G2)
    P1OUT |=  BIT4;                                                                // P1.4 setzen
    P1REN |= BIT4;                                                                 // P1.4 Pullup
    P1IE |= BIT4;                                                                  // P1.4 Interrupt aktivieren (nur für diesen Pin)
    P1IES |= BIT4;                                                                 // P1.4 Hi/lo Edge
    P1IFG &= ~BIT4;
    init_MCP2515_SPI();
    MCP2515_init();
    init_MCP2515_CANVariable(&can_tx);
    init_MCP2515_CANVariable(&can_rx);
    __enable_interrupt();

    while (1) {
        if(MCP2515_spi_test ()){
            P1OUT ^= BIT0;                                                               // P1.0 Togglen
            __delay_cycles(DELAY_1s);
        }

      //  read_ADC();



//        acc2New = 1023 - acc2Input;

////        if(APPS_Fault(acc1Input,acc2Input) || BSE_Fault(brakeInput,acc1Input,acc2Input)){
////            AcceleratorL = 0;
////            AcceleratorR = 0;
////            setThrottleValue(AcceleratorL,AcceleratorR);
////        }
//        if(BSE_Fault(brakeInput,acc2Input,acc2Input)){
//            AcceleratorL = 0;
//            AcceleratorR = 0;
//            setThrottleValue(AcceleratorL,AcceleratorR);
//        }
////        else{
////            setThrottleValue(AcceleratorL,AcceleratorR);
////        }
//        else {
//            int accel = 1023 - (acc2Input*4.5);
//            float B = 0.4; // offset of curve for torque vectoring
//            float A = 0.6/(72); // slope of curve for left  turn torque vectoring
//            float D = 0.6/(77); // slope of curve for right turn torque vectoring
////            if (acc1Input >= 512){
////                AcceleratorL = 1023*0.605;    //0.611 is conversion factor from digital value to duty cycle value
////                AcceleratorR = 1023*0.605;
////            }
////            else if(acc1Input > 201) {
////                AcceleratorL = 2*acc1Input*0.605;
////                AcceleratorR = 2*acc1Input*0.605;
////            }
////            else {
////                AcceleratorL = 0;
////                AcceleratorR = 0;
////            }
//            steeringval = steeringInput; //steering potentiometer value
//            //TORQUE VECTORING BASIC ALGORITHM
//            if (steeringval < 373 && steeringval > 84) { //left turn calibrated for 20% margine b4 turn engaging
//                AcceleratorR = accel *( A*((512-steeringval) - 84) + B)*0.605; //calibrated so it is 40% at full turn
//                AcceleratorL = accel*0.605;
//            }
//            else if (steeringval < 84){
//                AcceleratorR = 1023*0.605;
//                AcceleratorL = accel*0.605;
//            }
//            else if (steeringval > 534 && steeringval < 842) {                 //right turn calibrated for 20% margine
//                AcceleratorR = accel*0.605;
//                AcceleratorL = accel *(D *(842-(1023-steeringval)) + B)*0.605;  //calibrated so it is 40% at full turn
//
//            }
//            else if (steeringval > 842){
//                AcceleratorR = accel*0.605;
//                AcceleratorL = 1023*0.605;
//            }
//            else { //on center steering.
//                AcceleratorR = accel*0.605;
//                AcceleratorL = accel*0.605;
//            }
//
//            setThrottleValue(AcceleratorL,AcceleratorR);
//        }


    }
}
// Port 1: Interrupt-Service-Routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  MCP2515_can_rx0(&can_rx);                                                      // Read information in RX0
  __delay_cycles(DELAY_10ms);                                                    // Wait 10ms
  MCP2515_can_tx0(&can_tx);                                                      // Send the received info back
  P1IFG &= ~BIT4;                                                                // P1.4 IFG reset interrupt
}
