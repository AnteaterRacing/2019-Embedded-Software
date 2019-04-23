//##########################################################################################################################################################
// Anteater Electric Racing  "Driver Input Module"
//
// Engineer: Lucas Juttner
// Date: 4/23/2019
// School: University of California, Irvine
// File: SPI.c
//
// Description: Functions that initialize SPI communication, and provide all functions needed for CAN Communication
//
// Special thanks to ElectroDummies and K. Evangelos for their tutorial and sample code for CAN communication
// Their tutorial can be found here:  http://www.electrodummies.net/en/msp430-2/msp430-can-interface/
//##########################################################################################################################################################

#include <SPI.h>
#include <msp430g2553.h>
#include <mcp2515.h>
#include <stdint.h>
#include <ADC.h>

//----------------------------------------------- Function to Initialize SPI ------------------------------------------------------

void init_MCP2515_SPI(){

    P1SEL = BIT5 + BIT6 + BIT7;     //1.5 is clock source,  1.6 SOMI,   1.7 SIMO
    P1SEL2 = BIT5 + BIT6 + BIT7;

    P2DIR |= BIT0;
    P2OUT |= BIT0;                  //Set P2.0 as slave select pin

    UCB0CTL1 |= UCSWRST;                                       //USCI software reset (reset to begin initialization)
    UCB0CTL0 |= UCCKPL + UCMSB + UCMST + UCMODE_0 + UCSYNC;    //Most sig bit first, set as master, spi mode on
    UCB0CTL1 |= UCSSEL_2;                                      //Use SMCLK for clock source
    UCB0BR0 |= 0X02;                                           //Divide clock by 2
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST;                                      //initialize USCI state machine

    __delay_cycles(DELAY_100ms);
    while(!(IFG2 & UCB0TXIFG));
    UCB0TXBUF = 0x00;               //Initialize transfer buffer at 0x00
}

//----------------------------------------------- Function to Transmit via SPI ------------------------------------------------------

unsigned char transmit_MCP2515_SPI(unsigned char value){
    UCB0TXBUF = value;
    while(UCB0STAT & UCBUSY);
    return UCB0RXBUF;
}

//----------------------------------------------- Function to Reset MCP2515 chip ------------------------------------------------------
void MCP2515_reset(){
    MCP2515_CS_LOW;
    transmit_MCP2515_SPI(MCP2515_RESET);
    MCP2515_CS_HIGH;

    __delay_cycles(DELAY_100us);
}

//----------------------------------------------- Function to Initialize CAN Structs------------------------------------------------------

void init_MCP2515_CANVariable(can_t * can){
    can -> COB_ID = 0x001;
    can -> status = 0x01;
    can -> dlc = CAN_DLC;
    can -> rtr = CAN_RTR;
    can -> ext = CAN_EXTENDET;
//    can -> data[0] = 0x69;
    char i;
    for(i = 0; i < CAN_DLC; i++){
        can -> data[i] = 0x05;
    }
}

//----------------------------------------------- Function to Write to MCP2515 chip ------------------------------------------------------
void write_MCP2515(uint8_t addr, uint8_t data){
    MCP2515_CS_LOW;
    transmit_MCP2515_SPI(MCP2515_WRITE);
    transmit_MCP2515_SPI(addr);
    transmit_MCP2515_SPI(data);
    MCP2515_CS_HIGH;
   // __delay_cycles(DELAY_100us);
    __delay_cycles(DELAY_1ms);
}

//------------------------------------- Function to Write to multiple registers of MCP2515 chip ----------------------------------------------

void write_many_registers_MCP2515(uint8_t addr, uint8_t len, uint8_t *data){
  MCP2515_CS_LOW;
  transmit_MCP2515_SPI(MCP2515_WRITE);
  transmit_MCP2515_SPI(addr);
  char i;
  for(i = 0; i < len; i++)
  {
    transmit_MCP2515_SPI(*data);
    data++;
  }
  MCP2515_CS_HIGH;
  __delay_cycles(DELAY_100us);
}

//----------------------------------------------- Function to Read from MCP2515 chip ------------------------------------------------------

uint8_t read_MCP2515(uint8_t addr){
    uint8_t data;
    transmit_MCP2515_SPI(MCP2515_READ);
    transmit_MCP2515_SPI(addr);
    data = transmit_MCP2515_SPI(MCP2515_DUMMY);
    MCP2515_CS_HIGH;
    __delay_cycles(DELAY_100us);
    return data;
}

//----------------------------------------------- Function to Read from multiple registers of MCP2515 chip---------------------------------------------

void read_many_registers_MCP2515(uint8_t addr, uint8_t length, uint8_t *data){
    MCP2515_CS_LOW;
    transmit_MCP2515_SPI(MCP2515_WRITE);
    transmit_MCP2515_SPI(addr);
    char i;
    for(i = 1; i < length; i++){
        *data = transmit_MCP2515_SPI(MCP2515_DUMMY);
        data++;
    }
    MCP2515_CS_HIGH;
    __delay_cycles(DELAY_100us);
}

//----------------------------------------------- Function to Write CAN ID to MCP2515 chip ------------------------------------------------------

void write_id_MCP2515(uint8_t addr, BOOL ext, unsigned long id){
    uint16_t canid;
    uint8_t tbufdata[4];

    canid = (unsigned short)(id & 0x0ffff);

    if(ext == TRUE){
        tbufdata[MCP2515_EID0] = (uint8_t) (canid & 0xff);
        tbufdata[MCP2515_EID8] = (uint8_t) (canid / 256);
        canid = (uint16_t)(id / 0x10000);
        tbufdata[MCP2515_SIDL] = (uint8_t) (canid & 0x03);
        tbufdata[MCP2515_SIDL] +=  (uint8_t)((canid & 0x1c)*8);
        tbufdata[MCP2515_SIDL] |= MCP2515_TXBnSIDL_EXIDE;
        tbufdata[MCP2515_SIDH] = (uint8_t)(canid / 32);
    }
    else // Sonst hier auch genutzt Standart 11-Bit-Identifier (CAN 2.0A)
    {
      tbufdata[MCP2515_SIDH] = (uint8_t)(canid / 8);
      tbufdata[MCP2515_SIDL] = (uint8_t)((canid & 0x07)*32);
      tbufdata[MCP2515_EID0] = 0;
      tbufdata[MCP2515_EID8] = 0;
    } // else

    if(tbufdata[0] == 0xff) return;
    write_many_registers_MCP2515(addr, 4, tbufdata);

    __delay_cycles(DELAY_100us);
}

//----------------------------------------------- Function to Read ID from MCP2515 chip ------------------------------------------------------

void read_id_MCP2515(uint8_t addr, unsigned long* id){
  uint16_t ID_Low, ID_High;
  if(addr == MCP2515_RXB0SIDL)
  {
    ID_Low  = (read_MCP2515(MCP2515_RXB0SIDL) >> 5);
    ID_High = (read_MCP2515(MCP2515_RXB0SIDH) << 3);

    *id = (unsigned long)ID_Low | (unsigned long)ID_High;
  }
  else
  {
    ID_Low  = (read_MCP2515(MCP2515_RXB1SIDL) >> 5);
    ID_High = (read_MCP2515(MCP2515_RXB1SIDH) << 3);

    *id = (unsigned long)ID_Low | (unsigned long)ID_High;
  }
}

//----------------------------------------------- Function to Initialize MCP2515 chip ------------------------------------------------------

void MCP2515_init(void){

  MCP2515_reset ();                           // Reset MCP2515

  __delay_cycles(DELAY_10ms);                 // Allow to reset



  write_MCP2515(MCP2515_CANCTRL, 0x88);       //Set CAN Control register to configuration mode
  write_MCP2515(MCP2515_CANINTE, 0x03);       //Set CAN to enable interrupts
  write_MCP2515(MCP2515_TXB0CTRL, 0x03);      //Set high message priority in transfer control register

  write_MCP2515(MCP2515_CNF1,0x00);           //Sets 125kb/s transfer speed using 4MHz SPI clock
  write_MCP2515(MCP2515_CNF2,0xb9);
  write_MCP2515(MCP2515_CNF3,0x05);

  write_MCP2515(MCP2515_RXB0CTRL, 0x64);      //Receive Buffer 0 Control - receive all messages
  write_MCP2515(MCP2515_RXB1CTRL, 0x60);      //Receive Buffer 1 Control - receive all messages
  write_MCP2515(MCP2515_BFPCTRL, 0x00);       //Disable RxnBF pins
  write_MCP2515(MCP2515_TXRTSCTRL , 0x00);    //Disable RTS pins

  write_MCP2515(MCP2515_CANCTRL, 0x00);
}

//----------------------------------------------- Function to Modify single bit in register ------------------------------------------------------

void bit_modify_MCP2515(uint8_t addr, uint8_t mask, uint8_t data){
  MCP2515_CS_LOW;

  transmit_MCP2515_SPI(MCP2515_BIT_MODIFY);
  transmit_MCP2515_SPI(addr);
  transmit_MCP2515_SPI(mask);
  transmit_MCP2515_SPI(data);

  MCP2515_CS_HIGH;

  __delay_cycles(DELAY_100us);
}

//----------------------------------------------- Functions to Send CAN data ------------------------------------------------------

void MCP2515_can_tx0(can_t *can){
  if(can->dlc > 8) can->dlc = 8;

  write_id_MCP2515(MCP2515_TXB0SIDH, can->ext, can->COB_ID);

  if (can->rtr == TRUE)
  {
      uint8_t befehl = can->dlc;
      befehl = befehl | 0x40;
      if(befehl == 0x03) return;
      write_MCP2515(MCP2515_TXB0DLC, can->dlc | 0x40);
  }

  else
  {
    write_MCP2515(MCP2515_TXB0DLC, can->dlc);
    write_many_registers_MCP2515(MCP2515_TXB0D0, can->dlc, can->data);
    write_MCP2515(MCP2515_TXB0CTRL, 0x0B);
  }
}

void MCP2515_can_tx1(can_t *can){
  if(can->dlc > 8) can->dlc = 8;

  write_id_MCP2515(MCP2515_TXB1SIDH, can->ext, can->COB_ID);

  if (can->rtr == TRUE)
  {
      uint8_t befehl = can->dlc;
      befehl = befehl | 0x40;
      if(befehl == 0x03) return;
      write_MCP2515(MCP2515_TXB1DLC, can->dlc | 0x40);
  }

  else
  {
    write_MCP2515(MCP2515_TXB1DLC, can->dlc);
    write_many_registers_MCP2515(MCP2515_TXB1D0, can->dlc, can->data);
    write_MCP2515(MCP2515_TXB1CTRL, 0x0B);
  }
}

void MCP2515_can_tx2(can_t *can){
  if(can->dlc > 8) can->dlc = 8;

  write_id_MCP2515(MCP2515_TXB2SIDH, can->ext, can->COB_ID);

  if (can->rtr == TRUE)
  {
      uint8_t befehl = can->dlc;
      befehl = befehl | 0x40;
      if(befehl == 0x03) return;
      write_MCP2515(MCP2515_TXB2DLC, can->dlc | 0x40);
  }

  else
  {
    write_MCP2515(MCP2515_TXB2DLC, can->dlc);
    write_many_registers_MCP2515(MCP2515_TXB2D0, can->dlc, can->data);
    write_MCP2515(MCP2515_TXB2CTRL, 0x0B);
  }
}

//----------------------------------------------- Functions to Receive CAN data ------------------------------------------------------

void MCP2515_can_rx0(can_t *can){
  read_id_MCP2515(MCP2515_RXB0SIDL, &can->COB_ID);
  can->dlc = read_MCP2515(MCP2515_RXB0DLC);
  char i;
  for(i = 0; i < can->dlc; i++) can->data[i] = read_MCP2515(MCP2515_RXB0D0+i);
  can->status = can->data[0];

  MCP2515_clear_rx0();
  MCP2515_int_clear();

  __delay_cycles(DELAY_1ms);
}

void MCP2515_can_rx1(can_t *can){
  read_id_MCP2515(MCP2515_RXB1SIDL, &can->COB_ID);
  can->dlc = read_MCP2515(MCP2515_RXB1DLC);
  char i;
  for(i = 0; i < can->dlc; i++) can->data[i] = read_MCP2515(MCP2515_RXB1D0+i);
  can->status = can->data[0];

  MCP2515_clear_rx1();
  MCP2515_int_clear();

  __delay_cycles(DELAY_1ms);
}

//----------------------------------------------- Functions to Clear receive registers  ------------------------------------------------------

void MCP2515_clear_rx0(void){
  bit_modify_MCP2515(MCP2515_CANINTF, MCP2515_RX0IF, 0x00);
}

void MCP2515_clear_rx1(void){
  bit_modify_MCP2515(MCP2515_CANINTF, MCP2515_RX1IF, 0x00);
}

void MCP2515_int_clear(void){
  write_MCP2515(MCP2515_CANINTF, MCP2515_CANINTF_ALL_DISABLE);
}

//----------------------------------------------- Function to Test SPI communication ------------------------------------------------------

BOOL MCP2515_spi_test (void){
  uint16_t data_rcv[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint16_t data_snd[11]={0x88,0x03,0x90,0x02,0x05,0x02,0x3f,0x23,0x40,0x40,0x00};   //Data array

  write_MCP2515(MCP2515_CANCTRL, data_snd[0]);        //Transmit data from buffer to register, then read from register
  data_rcv[0] = read_MCP2515(MCP2515_CANCTRL);

  write_MCP2515(MCP2515_CNF1,data_snd[1]);
  write_MCP2515(MCP2515_CNF2,data_snd[2]);
  write_MCP2515(MCP2515_CNF3,data_snd[3]);
  data_rcv[1] = read_MCP2515(MCP2515_CNF1);
  data_rcv[2] = read_MCP2515(MCP2515_CNF2);
  data_rcv[3] = read_MCP2515(MCP2515_CNF3);
  write_MCP2515(MCP2515_RXM0SIDH, data_snd[4]);
  write_MCP2515(MCP2515_RXM0SIDL, data_snd[5]);
  data_rcv[4] = read_MCP2515(MCP2515_RXM0SIDH);
  data_rcv[5] = read_MCP2515(MCP2515_RXM0SIDL);
  write_MCP2515(MCP2515_CANINTE, data_snd[6]);
  data_rcv[6] = read_MCP2515(MCP2515_CANINTE);
  write_MCP2515(MCP2515_CANINTF, data_snd[7]);
  data_rcv[7] = read_MCP2515(MCP2515_CANINTF);
  write_MCP2515(MCP2515_TXB0SIDL, data_snd[8]);
  data_rcv[8] = read_MCP2515(MCP2515_TXB0SIDL);
  write_MCP2515(MCP2515_TXB1SIDL, data_snd[9]);
  data_rcv[9] = read_MCP2515(MCP2515_TXB1SIDL);

  write_MCP2515(MCP2515_CANCTRL, data_snd[10]);
  data_rcv[10] = read_MCP2515(MCP2515_CANCTRL);
  char i;
  for(i = 0; i < 11; i++){
    if(data_snd[i] != data_rcv[i]) return FALSE;    // Check to see if receive buffer = transfer buffer
  }

  MCP2515_init();
  return TRUE;                                      //Return TRUE if SPI functions correctly
}
