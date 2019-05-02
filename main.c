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

can_t can_tx;
can_t can_rx;
int startButton = 0;
int CAN_Data[5];
char ready= 0x00;
char fault = 0x00;
char throttle, brake, steering;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    BCSCTL1 = CALBC1_8MHZ; // Set range
    DCOCTL = CALDCO_8MHZ;
    init_ADC();
//   init_UART();
//   init_PWM();

    P1DIR |= BIT0;                                                                 // P1.0 Green LED
    P1OUT |=  BIT4;
    P1REN |= BIT4;
    P1IE |= BIT4;                                                                  // P1.4 Interrupt
    P1IES |= BIT4;                                                                 // P1.4 Interrupt on Hi/lo Edge
    P1IFG &= ~BIT4;

    P2SEL &= ~BIT1;
    P2DIR &= ~BIT1;
    P2IN &= ~BIT1;
    P2REN = BIT1;


    init_MCP2515_SPI();
    MCP2515_init();
    init_MCP2515_CANVariable(&can_tx);
    init_MCP2515_CANVariable(&can_rx);
    __enable_interrupt();

    while(1){
        read_ADC();
        //Calibrations for inputs
        if(brakeInput < 430){   //421 is resting
            brake = 0x00;
        }
        else{
            brake = brakeInput >> 2;
        }
        if(acc1Input < 420 || acc1Input > 690){   //416 is resting   679 is full press
            throttle = 0x00;
        }
        else{
            throttle = 0.958*(acc1Input - 409);
        }
        steering = steeringInput >> 2;


        if(P2IN & BIT1){
            startButton = 0;
        }
        else{
            startButton = 1;
        }
        if(brakeInput > 0xA0 && startButton==1){
            ready = 0xFF;
            while (1) {
                read_ADC();
        //      if(APPS_Fault(acc1Input,acc2Input)){
        //          fault=0xFF;
        //      }
        //      else{
        //          fault=0x00;
        //      }

                //Calibrations for inputs
                if(brakeInput < 430){   //421 is resting
                    brake = 0x00;
                }
                else{
                    brake = brakeInput >> 2;
                }
                if(acc1Input < 420 || acc1Input > 690){   //416 is resting   679 is full press
                    throttle = 0x00;
                }
                else{
                    throttle = 0.958*(acc1Input - 409);
                }
                steering = steeringInput >> 2;



                CAN_Data[0] = steering;
                CAN_Data[1] = brake;
                CAN_Data[2] = throttle;
                CAN_Data[3] = fault;
                CAN_Data[4] = ready;
//                P1OUT ^= 0;
 //               __delay_cycles(DELAY_1s);
                if(MCP2515_spi_test ()){
                    P1OUT ^= 0;                                                               // P1.0 Toggle
//                    __delay_cycles(DELAY_1s);
                }

            }
       }
        CAN_Data[0] = steering;
        CAN_Data[1] = brake;
        CAN_Data[2] = throttle;
        CAN_Data[3] = fault;
        CAN_Data[4] = ready;
        if(MCP2515_spi_test ()){
            P1OUT ^= 0;                                 // P1.0 Toggle
        }
    }
}
// Port 1: Interrupt-Service-Routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  MCP2515_can_rx0(&can_rx);                                                      // Read information in RX0
  //__delay_cycles(DELAY_10ms);                                                   // Wait 10ms
  int i;
  for(i = 0; i < 5; i++){
      can_tx.data[i] = CAN_Data[i];
  }
  MCP2515_can_tx0(&can_tx);                                                      // Send the received info back
  P1IFG &= ~BIT4;                                                                // P1.4 IFG reset interrupt
}
