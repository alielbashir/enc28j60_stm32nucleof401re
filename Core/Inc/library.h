#ifndef __LIBRARY_H
#define __LIBRARY_H

#include <stm32f4xx_hal.h>
#include <main.h>


void Spi_Enable(void);
void Spi_Disable(void);

void Set_Bank(uint8_t BANK_);

uint8_t Enc_Read_Operation(uint8_t operation, uint8_t address);
void Enc_Write_Operation(uint8_t operation, uint8_t address, uint8_t  data);

uint8_t Enc_Read_Cont_Reg8(uint8_t address);
void Enc_Write_Cont_Reg8(uint8_t address, uint8_t data);

uint16_t Enc_Read_Cont_Reg16(uint8_t address);
void Enc_Write_Cont_Reg16(uint8_t address, uint16_t data);

uint16_t Enc_Read_Cont_Reg16(uint8_t address,  uint8_t BANK_);
void Enc_Write_Cont_Reg16(uint8_t address_l, uint16_t dat,  uint8_t BANK_);


void Enc_INIT(void);


#define ENC28_READ_CTRL_REG          0x00
#define ENC28_READ_BUFFER_MEMORY	 0X01
#define ENC_REC_WRITE_REG 	         0x02
#define ENC_WRITE_BUFFER_MEMORY		 0X03
#define ENC28_SOFT_RESET 	         0x07
#define ENC28_READ_CTRL_REG          0x00
#define ENC28J60_BIT_FIELD_SET       0x04


#define BANK_0  0x00
#define BANK_1  0x01
#define BANK_2  0x02
#define BANK_3  0x03


#define TXSTART_INIT	 0x0
#define TXSTOP_INIT		(0x1fff-0x0600-1)
#define RXSTART_INIT	(0x1fff-0x0600)
#define RXSTOP_INIT		 0x1fff

#define MAX_FRAMELEN   1500
// Bank0 - control registers addresses
#define ERDPT		0x00
#define EWRPT		0x02
#define ETXST		0x04
#define ETXND		0x06
#define ERXST		0x08
#define ERXND		0x0A
#define ERXRDPT		0x0C
#define ERXWRPT		0x0E
#define ECON1_RXEN	0x04
#define EFLOCON		0x17

#define PADCFG1		0x01
#define TXCRCEN 0x10
#define FULDPX	0x01
#define MACON3_FRMLNEN 0x02
#define MAIPGL	0x06
#define MABBIPG 0x04
#define MAIPGL 0x06
#define MAIPGH 0x07
#define MAMXFLL	0x0a
#define PHCON2_HDLDIS 0x0100
#define PHCON2 0x10
#define EIE 0x1b
#define EIE_INTIE 0x80
#define EIE_PKTIE 0x40
#define MACON3 0x02
#define FCEN0	0x00
#define FCEN1	0x02
#define FULDPXS	0x04

//my MAC address  0x74,0x69, 0x69,0x2D,0x30,0x36
#define MAC_1			0xA8  //0x36
#define MAC_2			0xCD  //0x30
#define MAC_3			0x75  //0x2D
#define MAC_4			0xA9  //0x69
#define MAC_5			0x41  //0x69
#define MAC_6			0xDC  //0x74

#define MACON1	0x0

#define MAADR5  0x00
#define MAADR6  0x01
#define MAADR3	0x02
#define MAADR4	0x03
#define MAADR1 	0x04
#define MAADR2	0x05
/*
#define MAADR1           (0x00)
#define MAADR0           (0x01)
#define MAADR3           (0x02)
#define MAADR2           (0x03)
#define MAADR5           (0x04)
#define MAADR4           (0x05)
*/

// BitField Defines
#define ERXFCON									0x18
#define ECON1_BSEL0								0x01
#define ECON1_BSEL1								0x02
#define ESTAT_CLKRDY 							0x01
#define ECON2_PKTDEC							0x40
#define ECON2_AUTOINC							0x80
#define ECON1_RXEN								0x04
#define ECON1_TXRST								0x80
#define ECON1_TXRTS								0x08
#define ERXFCON_UCEN							0x80
#define ERXFCON_CRCEN							0x20
#define ERXFCON_PMEN							0x10
#define ERXFCON_BCEN							0x01
#define ERXFCON_ANDOR							0x40
#define MACON1_MARXEN							0x01
#define MACON1_TXPAUS							0x08
#define MACON1_RXPAUS							0x04
#define MACON1_PASSALL							0x02
#define MACON3_PADCFG0							0x20
#define MACON3_TXCRCEN							0x10
#define MACON3_FRMLNEN							0x02
#define EIE_INTIE								0x80
#define EIE_PKTIE								0x40
#define EIR_TXERIF								0x02
#define EIR_PKTIF 								0x40
#define EIR_TXIF								0x08
#define MICMD_MIIRD								0x01
#define BUFFER_REGISTER						0x1A

// Common registers
#define EIR											0x1C
#define ESTAT										0x1D
#define ECON1										0x1F
#define ECON2										0x1E


#endif /* __LIBRARY_H */

#ifdef stdio
	#include <stdio.h>
#endif

