#include <library.h>

extern SPI_HandleTypeDef hspi2;



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
	HAL_SPI_Transmit(&hspi2, &spiData[0], 1, 100);
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

	return Enc_Read_Operation(ENC28_READ_CTRL_REG , address); // choose operation that wanted

}


void Set_Bank(uint8_t BANK_){

	Enc_Write_Operation(ENC_REC_WRITE_REG, ECON1, BANK_); // bank is selected

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


void Enc_Read_Phy_Registers(void){

}

void Enc_Phy_Register_INIT(void)
{

}


void Enc_Write_Cont_Reg16(uint8_t address, uint16_t data)
{

	uint8_t datalow = (uint8_t)(data & 0x00FF);
	uint8_t datahigh = (uint8_t)((data & 0xFF00) >> 8);

	Enc_Write_Cont_Reg8(address, datalow);
	Enc_Write_Cont_Reg8((address + 1), datahigh);// shift for last 8 bits

}

void Enc_INIT()
{

	Spi_Enable();

	Enc_Write_Operation(ENC28_SOFT_RESET, 0x1F, 0x00);


	while (!Enc_Read_Operation(ENC28_READ_CTRL_REG, ESTAT) & ESTAT_CLKRDY);


	ENC_SET_BANK_0_REGISTERS();

	ENC_SET_BANK_1_REGISTERS();

	ENC_SET_BANK_2_REGISTERS();

	ENC_SET_BANK_3_REGISTERS();

	Spi_Disable();

}


void ENC_SET_BANK_0_REGISTERS(void)
{

	Set_Bank(BANK_0);

	// ETHERNET BUFFER ORGANIZATION
	// this bytes dedicated to RECEIVE Hardware

	// decide tx memory size
	Enc_Write_Cont_Reg16(ETXST, TXSTART_INIT);
	Enc_Write_Cont_Reg16(ETXND, TXSTOP_INIT);

	// decide rx memory size
	Enc_Write_Cont_Reg16(ERXST, RXSTART_INIT);
	Enc_Write_Cont_Reg16(ERXND, RXSTOP_INIT);


	Enc_Write_Cont_Reg16(ERXRDPT, RXSTART_INIT);


	Enc_Write_Cont_Reg16(ERXWRPT, RXSTART_INIT);

	//automatically increment to point to the next address after the last bit of each byte	Enc_Write_Cont_Reg8(ECON2, ECON2_AUTOINC);

	// no loopback of transmitted frames
	Enc_Write_Cont_Reg16(PHCON2, PHCON2_HDLDIS);

	// enable interrutps
	Enc_Write_Operation(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE);

	// enable packet reception
	Enc_Write_Operation(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);

}


void ENC_SET_BANK_1_REGISTERS(void)
{

	Set_Bank(BANK_1);

	// DESTINATION ADDRESS  FF-FF-FF-FF-FF-FF
	// enable the filters

	uint16_t erxfcon =  ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_PMEN|ERXFCON_BCEN;
	Enc_Write_Cont_Reg16(ERXFCON, erxfcon);

}


void ENC_SET_BANK_2_REGISTERS(void)
{

	Set_Bank(BANK_2);

	// enable MAC receive
	// enable automatic padding to 60bytes and CRC operations

	Enc_Write_Cont_Reg16(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);

	Enc_Write_Cont_Reg16(MACON3, PADCFG1|TXCRCEN|FULDPX|MACON3_FRMLNEN);

	Enc_Write_Cont_Reg16(MAMXFLL, MAX_FRAMELEN);


	Enc_Write_Cont_Reg8(MABBIPG, 0x15);

	Enc_Write_Cont_Reg16(MAIPGL, 0xC12);

}


void ENC_SET_BANK_3_REGISTERS(void)
{

	Set_Bank(BANK_3);

	Enc_Write_Cont_Reg8(MAADR5, MAC_5);
	Enc_Write_Cont_Reg8(MAADR6, MAC_6);
	Enc_Write_Cont_Reg8(MAADR3, MAC_3);
	Enc_Write_Cont_Reg8(MAADR4, MAC_4);
	Enc_Write_Cont_Reg8(MAADR1, MAC_1);
	Enc_Write_Cont_Reg8(MAADR2, MAC_2);

	//enable flow control
	Enc_Write_Cont_Reg8(EFLOCON, 0x06);

}


uint8_t Enc_Read_Buffer_toMemory(uint8_t address)
{

	uint8_t spiData[2];

	spiData[0] = (ENC_WRITE_BUFFER_MEMORY << 5) | address; // last 3 bits are operation, first 5 bit of a byte is argument, shown above

	Spi_Enable();
	HAL_SPI_Transmit(&hspi2, &spiData[0], 1, 100);
	HAL_SPI_Receive(&hspi2, &spiData[1], 1, 100);
	Spi_Disable();

	return spiData[1];

}


void Enc_Write_Buffer_toMemory(uint8_t *data)
{

	uint8_t spiData[2] = {(ENC_REC_WRITE_REG << 5) | BUFFER_REGISTER, 0xFF};

	Spi_Enable();

	HAL_SPI_Transmit(&hspi2, &spiData[0], 1, 100);

	//HAL_SPI_Transmit(&hspi2, data, sizeof(data), 100);

	do
	{

		HAL_SPI_Transmit(&hspi2, data, sizeof(data), 100);

	}while(*data++);

	Spi_Disable();

}


void Spi_Enable(void)
{

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // set cs pin low

}


void Spi_Disable(void)
{

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);	  // set cs pin high

}


