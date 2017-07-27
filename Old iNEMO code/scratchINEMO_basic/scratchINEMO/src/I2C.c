/*
 * I2C.c
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */
//remember to set up the registers for the I2C device, see AccMag.c for examples on this
//change the #define variables in the I2C.h file
//be sure to change all the addresses and also to check if the data is 2's compliment
#include "I2C.h"

void initI2C(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable I2C and GPIO clocks */
	RCC_APB1PeriphClockCmd(I2Cclock, ENABLE);
	RCC_APB2PeriphClockCmd(I2CportClock|RCC_APB2Periph_AFIO, ENABLE);

	/* Configure I2C pins: SCL and SDA */
	GPIO_InitStructure.GPIO_Pin =  I2Cscl|I2Csda;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(I2Cport, &GPIO_InitStructure);

	/* I2C configuration */
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;

	/* Apply I2C configuration after enabling it */
	I2C_Init(I2C1, &I2C_InitStructure);

	/* I2C Peripheral Enable */
	I2C_Cmd(I2C1, ENABLE);
}
void I2CRead(u8 address,uint32_t numByteToRead,u8* pBuffer)
{
	while((I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY)));
	I2C_GenerateSTART(I2C1,ENABLE);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1,(u8)0x32,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_Cmd(I2C1,ENABLE);
	I2C_SendData(I2C2,(u8)address);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTART(I2C1,ENABLE);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1,(u8)0x33,I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	while(numByteToRead)
	{

		if(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			*pBuffer=I2C_ReceiveData(I2C1);
			pBuffer++;
			numByteToRead--;
		}
		if(numByteToRead==1)
		{
			I2C_AcknowledgeConfig(I2C1,DISABLE);
			I2C_GenerateSTOP(I2C1,ENABLE);
		}
	}
	I2C_AcknowledgeConfig(I2C1,ENABLE);
}
void I2CWrite(u8 data, u8 address)
{
	while((I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY)));
	I2C_GenerateSTART(I2C1,ENABLE);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1,(u8)0x32,I2C_Direction_Transmitter);//slave address
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C1,(u8)address);//write address
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2C1,(u8)data);//data
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(I2C1,ENABLE);
}
