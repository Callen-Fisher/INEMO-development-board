/*
 * SPI.c
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */

#include "SPI.h"

//to set up the SPI change the #define variables in the SPI.h file, also remember
//to call initSPI() in the main method
//please remember to set up the registers for the device

//you shouldnt need to change anything in this c file unless the SPI device is not 8
//bit and isnt MSB first.


void initSPI(void)
{
	RCC_APB2PeriphClockCmd(CSPortClock,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);


	GPIO_InitTypeDef GPIO_InitStruct;

	/* Pack the struct */
	GPIO_InitStruct.GPIO_Pin =SPIMosi|SPIMiso|SPISck;
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(SPIPort, &GPIO_InitStruct);

	//Init CS-Pin
	GPIO_InitStruct.GPIO_Pin =CSPin;
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(CSPort, &GPIO_InitStruct);

	GPIO_SetBits(CSPort, CSPin);  //Set CS high

	SPI_InitTypeDef  SPI_InitStructure;

	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b ;
	SPI_InitStructure.SPI_CPOL = SPIPol;
	SPI_InitStructure.SPI_CPHA = SPIPhase;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);
}
int32_t writeSPI(uint8_t regAdr, uint8_t data)//also reads the register
{
	uint8_t dummyVar;
	int32_t val;

	/* Pull CS line low */
	GPIO_ResetBits(CSPort, CSPin);

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	SPI_I2S_SendData(SPI1, regAdr);    //Sensor Address that we are WRITING to
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
	{
		/* Wait for data */
	}
	dummyVar = SPI_I2S_ReceiveData(SPI1);

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	SPI_I2S_SendData(SPI1, data);    //Sensor Config

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		/* Wait for data */
	}
	val =(int8_t) SPI_I2S_ReceiveData(SPI1);

	/* Pull CS line high */
	GPIO_SetBits(CSPort, CSPin);
	return val;

}
