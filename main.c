//##########################################################################################################################################################
// Anteater Electric Racing  "Driver Input Module"
//
// Engineer: Lucas Juttner
// Date: 4/23/2019
// School: University of California, Irvine
//
// Description: Program takes in analog input from four potentiometers
//              (two from accelerator pedal, one from brake pedal, and one from steering column)
//              Analog input values are sent over CANBus along with a "ready" flag
//              and a "fault" flag. "Ready" flag is set when brake pedal is pressed
//              at the same time as start button. "Fault" flag is described in rule T.6.2 in the
//              FSAE 2019 rulebook.
//
//##########################################################################################################################################################

#include <ADC.h>
#include <Faults.h>
#include <msp430g2553.h>
#include <PWM.h>
#include <SPI.h>
#include <stdint.h>
#include <UART.h>
#include <mcp2515.h>

//############################################### Global Variable initializations ##########################################################################

can_t can_tx;           //CAN transfer struct
can_t can_rx;           //CAN receive struct
int startButton = 0;    //start button flag
int CAN_Data[5];
char ready= 0x00;
char fault = 0x00;

//############################################### Start of main function ###################################################################################
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    BCSCTL1 = CALBC1_8MHZ; // Set range
    DCOCTL = CALDCO_8MHZ;
    init_ADC();
//   init_UART();
//   init_PWM();

    P1DIR |= BIT0;                    // P1.0 Green LED

//----------------------------------------------- Configure P1.4 as interrupt pin --------------------------------------------------------------------------

    P1OUT |=  BIT4;
    P1REN |= BIT4;
    P1IE |= BIT4;              // P1.4 Interrupt Enable
    P1IES |= BIT4;             // P1.4 Interrupt on Hi/lo Edge
    P1IFG &= ~BIT4;

//----------------------------------------------- Configure P2.1 as digital input for start button------------------------------------------------------

    P2SEL &= ~BIT1;
    P2DIR &= ~BIT1;
    P2IN &= ~BIT1;
    P2REN = BIT1;

//----------------------------------------------- Initiate SPI and CAN structs --------------------------------------------------------------------------

    init_MCP2515_SPI();
    MCP2515_init();
    init_MCP2515_CANVariable(&can_tx);
    init_MCP2515_CANVariable(&can_rx);
    __enable_interrupt();

//----------------------------------------------- Beginning of primary while loop --------------------------------------------------------------------------

    while(1){
        read_ADC();                 //Call function to read analog to digital input values
        if(P2IN & BIT1){            //If input is high on P2.1,
            startButton = 1;        //set startButton flag to high(1)
        }
        if((brakeInput >> 2) > 0x0F && startButton==1){     //If brake is pressed, and start button is on,
            ready = 0xFF;                                   //set "ready" flag to high(0xFF)
            while (1) {                                     //Enter new while loop that will continue until reset
                read_ADC();                                 //Call function to read analog to digital input values
        //      if(APPS_Fault(acc1Input,acc2Input)){        //If fault occurs, set "fault" flag to high(0xFF)
        //          fault = 0xFF;
        //      }
                CAN_Data[0] = steeringInput >> 2;           //Fill CAN data buffer with appropriate data values
                CAN_Data[1] = brakeInput >> 2;
                CAN_Data[2] = acc1Input >> 2;
                CAN_Data[3] = ready;
                CAN_Data[4] = fault;
                if(MCP2515_spi_test ()){                    //Validate SPI communication functions properly
                    P1OUT ^= 0;                             // P1.0 toggle
                }

            }
        }
        else{                                               //If start sequence isn't initiated, send CAN data regardlesss
            CAN_Data[0] = steeringInput >> 2;
            CAN_Data[1] = brakeInput >> 2;
            CAN_Data[2] = acc1Input >> 2;
            CAN_Data[3] = ready;
            CAN_Data[4] = fault;
            if(MCP2515_spi_test ()){
                P1OUT ^= 0;                                 // P1.0 Toggle
            }
        }
    }
}


//############################################### Port1: Interrupt-Service-Routine ###################################################################################
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  MCP2515_can_rx0(&can_rx);                   // Read information in RX0
  int i;
  for(i = 0; i < 5; i++){
      can_tx.data[i] = CAN_Data[i];
  }
  MCP2515_can_tx0(&can_tx);                  // Send CAN message
  P1IFG &= ~BIT4;                            // P1.4 IFG reset interrupt
}
