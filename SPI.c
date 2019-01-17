#include <SPI.h>
#include <msp430g2553.h>

void init_SPI(){
    P2OUT |= BIT0;                  //slave select pin
    P2DIR |= BIT0;
    P1SEL |= BIT5 + BIT6 + BIT7;    //1.5 is clock source  1.6 SOMI   1.7 SIMO

    UCB0CTL1 = UCSWRST;     //USCI software reset (reset to begin initialization)
    UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;    //Most sig bit first, set as master, spi mode on
    UCB0CTL1 |= UCSSEL_2;   //use SMCLK
    UCB0BR0 |= 0X02;    //clock divider
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST; //initialize USCI state machine
    P2OUT &= ~BIT0;   //slave select (we only have one slave)
}

void transmit_SPI(int * data){
   int i = 0;
   while(!(IFG2 & UCB0TXIFG)); //while interrupt and transfer are not busy
   while(data[i]){              // cycles through data array to send all necessary values
       UCB0TXBUF = data[i];     //TXbuffer register getting values from data array
       i++;
   }
   while(!(IFG2 & UCB0RXIFG)); //while interrupt and receiving are not busy
   //received_ch = UCBA0RXBUF;
  // P2OUT |= BIT0;               //deselects selected slave
}
