//##########################################################################################################################################################
// Anteater Electric Racing  "Driver Input Module"
//
// Engineer: Lucas Juttner
// Date: 4/23/2019
// School: University of California, Irvine
// File: mcp2515.h
//
// Description: Header/driver file with all register definitions for MCP2515 CAN Transceiver
//
// Special thanks to ElectroDummies and K. Evangelos for their tutorial and sample code for CAN communication
// Their tutorial and code can be found here:  http://www.electrodummies.net/en/msp430-2/msp430-can-interface/
//##########################################################################################################################################################

#ifndef MCP2515_H
#define MCP2515_H

#include <msp430G2553.h>
#include <stdint.h>

//########################## General Definitions ###########################################################################################################

#define F_CPU         2000000            //Code to set delays, will have to change F_CPU value for different clock speeds
#define DELAY_1s    F_CPU
#define DELAY_100ms (F_CPU / 10)
#define DELAY_10ms  (F_CPU / 100)
#define DELAY_1ms   (F_CPU / 1000)
#define DELAY_100us (F_CPU / 10000)
#define DELAY_10us  (F_CPU / 100000)
#define DELAY_1us   (F_CPU / 1000000)

#define BOOL char
#define TRUE 0
#define FALSE 1

#define CAN_RTR FALSE        //Data Request = 1
#define CAN_DLC 5            //Number of bytes in the CAN message (maximum of 8)
#define CAN_EXTENDET FALSE   //Only receive standard IDs   DataRequest=1

#define MCP2515_CS_LOW   P2OUT &=~ BIT0         //Set chip select variables
#define MCP2515_CS_HIGH  P2OUT |=  BIT0         //Set chip select variables

//############################# MCP2515 Commands ###################################################################################################################################

// -------------------------- Basic Commands --------------------------------------------------

#define MCP2515_WRITE           0x02
#define MCP2515_READ            0x03
#define	MCP2515_READ_RX         0x90
#define	MCP2515_WRITE_TX        0x40
#define MCP2515_DUMMY           0xFF
#define MCP2515_RESET           0xC0
#define MCP2515_BIT_MODIFY      0x05
#define MCP2515_READ_STATUS     0xA0
#define	MCP2515_RX_STATUS       0xB0
#define	MCP2515_RTS	            0x80

// ------------------------------- Register Values -------------------------------------------------

#define MCP2515_RXF0SIDH	0x00
#define MCP2515_RXF0SIDL	0x01
#define MCP2515_RXF0EID8	0x02
#define MCP2515_RXF0EID0	0x03
#define MCP2515_RXF1SIDH	0x04
#define MCP2515_RXF1SIDL	0x05
#define MCP2515_RXF1EID8	0x06
#define MCP2515_RXF1EID0	0x07
#define MCP2515_RXF2SIDH	0x08
#define MCP2515_RXF2SIDL	0x09
#define MCP2515_RXF2EID8	0x0A
#define MCP2515_RXF2EID0	0x0B
#define MCP2515_BFPCTRL		0x0C
#define MCP2515_TXRTSCTRL	0x0D
#define MCP2515_CANSTAT		0x0E
#define MCP2515_CANCTRL		0x0F

#define MCP2515_RXF3SIDH	0x10
#define MCP2515_RXF3SIDL	0x11
#define MCP2515_RXF3EID8	0x12
#define MCP2515_RXF3EID0	0x13
#define MCP2515_RXF4SIDH	0x14
#define MCP2515_RXF4SIDL	0x15
#define MCP2515_RXF4EID8	0x16
#define MCP2515_RXF4EID0	0x17
#define MCP2515_RXF5SIDH	0x18
#define MCP2515_RXF5SIDL	0x19
#define MCP2515_RXF5EID8	0x1A
#define MCP2515_RXF5EID0	0x1B
#define MCP2515_TEC		    0x1C
#define MCP2515_REC		    0x1D

#define MCP2515_RXM0SIDH	0x20
#define MCP2515_RXM0SIDL	0x21
#define MCP2515_RXM0EID8	0x22
#define MCP2515_RXM0EID0	0x23
#define MCP2515_RXM1SIDH	0x24
#define MCP2515_RXM1SIDL	0x25
#define	MCP2515_RXM1EID8	0x26
#define MCP2515_RXM1EID0	0x27
#define MCP2515_CNF3		0x28
#define MCP2515_CNF2		0x29
#define MCP2515_CNF1		0x2A
#define MCP2515_CANINTE		0x2B
#define MCP2515_CANINTF		0x2C
#define MCP2515_EFLG		0x2D

#define	MCP2515_TXB0CTRL	0x30
#define	MCP2515_TXB0SIDH	0x31
#define MCP2515_TXB0SIDL	0x32
#define MCP2515_TXB0EID8	0x33
#define MCP2515_TXB0EID0	0x34
#define MCP2515_TXB0DLC		0x35
#define MCP2515_TXB0D0		0x36
#define MCP2515_TXB0D1		0x37
#define MCP2515_TXB0D2		0x38
#define MCP2515_TXB0D3		0x39
#define MCP2515_TXB0D4		0x3A
#define MCP2515_TXB0D5		0x3B
#define MCP2515_TXB0D6		0x3C
#define MCP2515_TXB0D7		0x3D

#define	MCP2515_TXB1CTRL	0x40
#define	MCP2515_TXB1SIDH	0x41
#define	MCP2515_TXB1SIDL	0x42
#define	MCP2515_TXB1EID8	0x43
#define	MCP2515_TXB1EID0	0x44
#define	MCP2515_TXB1DLC		0x45
#define	MCP2515_TXB1D0		0x46
#define	MCP2515_TXB1D1		0x47
#define	MCP2515_TXB1D2		0x48
#define	MCP2515_TXB1D3		0x49
#define	MCP2515_TXB1D4		0x4A
#define	MCP2515_TXB1D5		0x4B
#define	MCP2515_TXB1D6		0x4C
#define	MCP2515_TXB1D7		0x4D

#define	MCP2515_TXB2CTRL	0x50
#define	MCP2515_TXB2SIDH	0x51
#define	MCP2515_TXB2SIDL	0x52
#define	MCP2515_TXB2EID8	0x53
#define	MCP2515_TXB2EID0	0x54
#define	MCP2515_TXB2DLC		0x55
#define	MCP2515_TXB2D0		0x56
#define	MCP2515_TXB2D1		0x57
#define	MCP2515_TXB2D2		0x58
#define	MCP2515_TXB2D3		0x59
#define	MCP2515_TXB2D4		0x5A
#define	MCP2515_TXB2D5		0x5B
#define	MCP2515_TXB2D6		0x5C
#define	MCP2515_TXB2D7		0x5D

#define	MCP2515_RXB0CTRL    0x60
#define	MCP2515_RXB0SIDH	0x61
#define	MCP2515_RXB0SIDL	0x62
#define	MCP2515_RXB0EID8	0x63
#define	MCP2515_RXB0EID0	0x64
#define	MCP2515_RXB0DLC		0x65
#define	MCP2515_RXB0D0		0x66
#define	MCP2515_RXB0D1		0x67
#define	MCP2515_RXB0D2		0x68
#define	MCP2515_RXB0D3		0x69
#define	MCP2515_RXB0D4		0x6A
#define	MCP2515_RXB0D5		0x6B
#define	MCP2515_RXB0D6		0x6C
#define	MCP2515_RXB0D7		0x6D

#define	MCP2515_RXB1CTRL	0x70
#define	MCP2515_RXB1SIDH	0x71 
#define	MCP2515_RXB1SIDL	0x72
#define	MCP2515_RXB1EID8	0x73
#define	MCP2515_RXB1EID0	0x74
#define	MCP2515_RXB1DLC		0x75
#define	MCP2515_RXB1D0		0x76
#define	MCP2515_RXB1D1		0x77
#define	MCP2515_RXB1D2		0x78
#define	MCP2515_RXB1D3		0x79
#define	MCP2515_RXB1D4		0x7A
#define	MCP2515_RXB1D5		0x7B
#define	MCP2515_RXB1D6		0x7C
#define	MCP2515_RXB1D7		0x7D

// --------------------- Bit Defintions --------------------------------------------------------------------------

//Register BFPCTRL
#define	MCP2515_B1BFS 	0x5
#define	MCP2515_B0BFS 	0x4
#define	MCP2515_B1BFE 	0x3
#define	MCP2515_B0BFE	0x2
#define	MCP2515_B1BFM	0x1
#define	MCP2515_B0BFM	0x0

//Register TXRTSCTRL
#define	MCP2515_B2RTS	0x20
#define	MCP2515_B1RTS	0x10
#define	MCP2515_B0RTS	0x08
#define	MCP2515_B2RTSM	0x04
#define	MCP2515_B1RTSM	0x02
#define	MCP2515_B0RTSM	0x01

//Register CANSTAT
#define	MCP2515_OPMOD2	0x7
#define	MCP2515_OPMOD1	0x6
#define	MCP2515_OPMOD0	0x5
#define	MCP2515_ICOD2	0x3
#define	MCP2515_ICOD1	0x2
#define	MCP2515_ICOD0	0x1

//Register CANCTRL
#define	MCP2515_REQOP2	0x7
#define	MCP2515_REQOP1	0x6
#define	MCP2515_REQOP0	0x5
#define	MCP2515_ABAT	0x4	
#define	MCP2515_CLKEN	0x2
#define	MCP2515_CLKPRE1	0x1
#define	MCP2515_CLKPRE0	0x0

//Register CNF3
#define	MCP2515_WAKFIL	0x6
#define	MCP2515_PHSEG22	0x2
#define	MCP2515_PHSEG21	0x1
#define	MCP2515_PHSEG20	0x0

//Register CNF2
#define	MCP2515_BTLMODE	0x7
#define	MCP2515_SAM	0x6
#define	MCP2515_PHSEG12	0x5
#define	MCP2515_PHSEG11	0x4
#define	MCP2515_PHSEG10	0x3
#define	MCP2515_PRSEG2	0x2
#define	MCP2515_PRSEG1	0x1
#define	MCP2515_PRSEG0	0x0

//Register CNF1
#define	MCP2515_SJW1	0x7
#define	MCP2515_SJW0	0x6
#define	MCP2515_BRP5	0x5
#define	MCP2515_BRP4	0x4
#define	MCP2515_BRP3	0x3
#define	MCP2515_BRP2	0x2
#define	MCP2515_BRP1	0x1
#define	MCP2515_BRP0	0x0

//Register CANINTE
#define	MCP2515_MERRE	0x7
#define	MCP2515_WAKIE	0x6
#define	MCP2515_ERRIE	0x5
#define	MCP2515_TX2IE	0x4
#define	MCP2515_TX1IE	0x3
#define	MCP2515_TX0IE	0x2
#define	MCP2515_RX1IE	0x1
#define	MCP2515_RX0IE	0x0

//Register CANINTF
#define	MCP2515_MERRF	0x7
#define	MCP2515_WAKIF	0x6
#define	MCP2515_ERRIF	0x5
#define	MCP2515_TX2IF	0x4
#define	MCP2515_TX1IF	0x3
#define	MCP2515_TX0IF	0x2
#define	MCP2515_RX1IF	0x1
#define	MCP2515_RX0IF	0x0

//Register EFLG
#define	MCP2515_RX1OVR	0x7
#define	MCP2515_RX0OVR	0x6
#define	MCP2515_TXBO	0x5	
#define	MCP2515_TXEP	0x4
#define	MCP2515_RXEP	0x3
#define	MCP2515_TXWAR	0x2
#define	MCP2515_RXWAR	0x1
#define	MCP2515_EWARN	0x0

//Register TXB0CTRL, TXB1CTRL, TXB2CTRL
#define	MCP2515_ABTF	0x40
#define	MCP2515_MLOA	0x20
#define	MCP2515_TXERR	0x10
#define	MCP2515_TXREQ	0x08
#define	MCP2515_TXP1	0x02
#define	MCP2515_TXP0	0x01

//Register RXB0CTRL
#define	MCP2515_RXM1	0x6
#define	MCP2515_RXM0	0x5
#define	MCP2515_RXRTR	0x3
#define	MCP2515_BUKT	0x2
#define	MCP2515_BUKT1	0x1
#define	MCP2515_FILHIT0	0x0

//Register RXB1CTRL
#define	MCP2515_FILHIT2	0x2
#define	MCP2515_FILHIT1	0x1
#define	MCP2515_FILHIT0	0x0

#define MCP2515_SIDH    0x00
#define MCP2515_SIDL    0x01
#define MCP2515_EID8    0x02
#define MCP2515_EID0    0x03

#define MCP2515_TXBnSIDL_EXIDE      0x3

#define MCP2515_CAN_EXT_ID    1
#define MCP2515_CAN_NO_EXT_ID 0

#define MCP2515_RXB1CTRL_FILHIT_RXF5  0x05
#define MCP2515_RXB1CTRL_FILHIT_RXF4  0x04
#define MCP2515_RXB1CTRL_FILHIT_RXF3  0x03
#define MCP2515_RXB1CTRL_FILHIT_RXF2  0x02
#define MCP2515_RXB1CTRL_FILHIT_RXF1  0x01
#define MCP2515_RXB1CTRL_FILHIT_RXF0  0x00

#define MCP2515_CANINTF_ALL_DISABLE 0x00

#define MCP2515_EFLAG 0x2d

//################################## CAN Variables ####################################################################################################################################

#define CAN_RTR FALSE
//#define CAN_DLC 8
#define CAN_EXTENDET FALSE 

//########################### CAN struct Variables ######################################################################################################################################

typedef struct 
{
  uint32_t COB_ID;         //CAN ID number
  uint8_t status;
  uint8_t dlc;             //CAN data length(in bytes) - 8 max
  uint8_t rtr;             //CAN request to receive
  uint8_t ext;             //CAN extended data frame
  uint8_t data[CAN_DLC];   //CAN data buffer
} can_t;


#endif
