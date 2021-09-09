#include "library.h"

extern SPI_HandleTypeDef hspi2;

void Enc_Set_Bank(uint8_t BANK_)
{
	Enc_Write_Operation(ENC_REC_WRITE_REG, ECON1, BANK_); // bank is selected
}


uint8_t Enc_Read_Operation(uint8_t operation, uint8_t address)
{
	// shift 5 times (page 28 "SPI INSTRUCTION SET")
	//									{ Byte 0  }		 |	{   Byte 1  }
	//			First byte		Opcode 		Argument 	 |	Data
	// Read Control Register	0 0 0		a a a a a	 |	N\A
	// Read Buffer Memory		0 0 1		1 1 0 1 0    |	N\A
	// Write Control Register	0 1 0		a a a a a    |	d d d d d d d d
	// Write Buffer Memory		0 1 1		1 1 0 1 0    |	d d d d d d d d
	// Bit Field Set			1 0 0		a a a a a    |	d d d d d d d d
	// Bit Field Clear			1 0 1		a a a a a    |	d d d d d d d d
	// System Reset Command 	1 1 1		1 1 1 1 1    |	N\A
	//                          7 6 5       4 3 2 1 0	 |

	uint8_t spiData[2];

	spiData[0] = (operation << 5) | address; // last 3 bits are operation, first 5 bit of a byte is argument, shown above

	Spi_Enable();
	HAL_SPI_Transmit(&hspi2, spiData, 1, 100);
	HAL_SPI_Receive(&hspi2, &spiData[1], 1, 100);
	Spi_Disable();

	return spiData[1];
}


void Enc_Write_Operation(uint8_t operation, uint8_t address, uint8_t  data)
{

	uint8_t spiData[2];

	Spi_Enable();

	spiData[0] = (operation << 5) | address;
	spiData[1] = data;

	HAL_SPI_Transmit(&hspi2, spiData, 2, 100);

	Spi_Disable();

}


uint8_t Enc_Read_Cont_Reg8(uint8_t address)
{

	return Enc_Read_Operation(ENC28_READ_CTRL_REG , address); 		  // choose operation that wanted

}


void Enc_Write_Cont_Reg8(uint8_t address, uint8_t data)
{

	Enc_Write_Operation(ENC_REC_WRITE_REG, address, data); // choose operation that wanted

}

uint16_t Enc_Read_Cont_Reg16(uint8_t address)
{

	uint16_t data_16bit = 0x00;

	data_16bit |= (uint16_t)(Enc_Read_Cont_Reg8(address));
	data_16bit |= (uint16_t)(Enc_Read_Cont_Reg8(address + 1) << 8);

	return data_16bit;

}

void Enc_Write_Cont_Reg16(uint8_t address_l, uint16_t data)
{

	uint8_t datalow = (uint8_t)(data & 0x00FF);
	uint8_t datahigh = (uint8_t)((data & 0xFF00) >> 8);

	Enc_Write_Cont_Reg8(address_l, datalow);
	Enc_Write_Cont_Reg8((address_l + 1), datahigh);// shift for last 8 bits

}

void Enc_INIT(void)
{
//	Spi_Enable();
//
//	Enc_Write_Operation(ENC28_SOFT_RESET, 0x1F, 0x00);
//
//
//	while (!Enc_Read_Operation(ENC28_READ_CTRL_REG, ESTAT) & ESTAT_CLKRDY);
//
//	// ETHERNET BUFFER ORGANIZATION
//	// this bytes dedicated to RECEIVE Hardware
//	Enc_Write_Cont_Reg16(ERXST, RXSTART_INIT, BANK_0);
//	Enc_Write_Cont_Reg16(ERXST, RXSTOP_INIT, BANK_0);
//
//	Enc_Write_Cont_Reg16(ETXST, TXSTART_INIT, BANK_0);
//	Enc_Write_Cont_Reg16(ETXND, TXSTOP_INIT, BANK_0);
//
//
//	Enc_Write_Cont_Reg16(ERXRDPT, RXSTART_INIT, BANK_0);
//	Enc_Write_Cont_Reg16(ERXWRPT, RXSTART_INIT, BANK_0);
//
//
//	// DESTINATION ADDRESS  FF-FF-FF-FF-FF-FF
//	// enable the filters
//
//	Enc_Write_Cont_Reg16(ERXFCON, ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_PMEN|ERXFCON_BCEN, BANK_1);
//
//
//	// enable MAC receive
//	// enable automatic padding to 60bytes and CRC operations
//	//
//	Enc_Write_Cont_Reg16(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS, BANK_2);
//
//	Enc_Write_Cont_Reg16(MACON3, PADCFG1|TXCRCEN|FULDPX|MACON3_FRMLNEN, BANK_2);
//
//	Enc_Write_Cont_Reg16(MAMXFLL, MAX_FRAMELEN, BANK_2);
//
//
//	Enc_Write_Cont_Reg8(MABBIPG, 0x15, BANK_2);
//
//
//	Enc_Write_Cont_Reg16(MAIPGL, 0xC12, BANK_2);
//
//	Enc_Write_Cont_Reg8(MAADR5, MAC_5, BANK_3);
//	Enc_Write_Cont_Reg8(MAADR6, MAC_6, BANK_3);
//	Enc_Write_Cont_Reg8(MAADR3, MAC_3, BANK_3);
//	Enc_Write_Cont_Reg8(MAADR4, MAC_4, BANK_3);
//	Enc_Write_Cont_Reg8(MAADR1, MAC_1, BANK_3);
//	Enc_Write_Cont_Reg8(MAADR2, MAC_2, BANK_3);
//
//	// no loopback of transmitted frames
//	Enc_Write_Cont_Reg16(PHCON2, PHCON2_HDLDIS, BANK_0);
//
//	// switch to bank 0
//	Enc_Write_Operation(ENC_REC_WRITE_REG, ECON1, BANK_0); // bank is selected
//
//	// enable interrutps
//	Enc_Write_Operation(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE);
//
//	// enable packet reception
//	Enc_Write_Operation(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);
//
//	//enable flow control
//	Enc_Write_Cont_Reg8(EFLOCON, 0x06, BANK_3);

}


void Spi_Enable(void)
{

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // set cs pin low

}

void Spi_Disable(void)
{

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);	  // set cs pin high

}
