/*
 * gyro.c
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */

#include "gyro.h"
//////////////////////////////////////////////////////////////////////////
////////////////GYRO FUNCTIONS////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void setUpGyro()
{
	initSPIgyro();
	spiGyroRegisterSetup();
}

void initSPIgyro(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);


	GPIO_InitTypeDef GPIO_InitStruct;

	/* Pack the struct */
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_13 | GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	//Init CS-Pin
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_SetBits(GPIOB, GPIO_Pin_12);  //Set CS high

	SPI_InitTypeDef  SPI_InitStructure;

	SPI_I2S_DeInit(SPI2);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b ;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPI2, &SPI_InitStructure);

	SPI_Cmd(SPI2, ENABLE);
}
void spiGyroRegisterSetup(void)
{
	//l3gd20
	//DR1 DR0 BW1 BW0 PD Zen Xen Yen
	//DR1-DR0 data rate
	//BW1 BW0 bandwidth
	//PD power down 1=normal mode/sleep 0=power down
	//Zen Xen Yen 1=enable 0=disable

	//DR  BW  ODR(output data rate)
	//00 00=95
	//00 01=95
	//00 10=95
	//00 11=95
	//01 00=190
	//01 01=190
	//01 10=190
	//01 11=190
	//10 00=380
	//10 01=380
	//10 10=380
	//10 11=380
	//11 00=760
	//11 01=760
	//11 10=760
	//11 11=760
	(void)writeSPIgyro(0b00100000,0b11111111);//reg 0x20-ctrl_Reg1 (first 2 bits are "write" and "no increment"

	//interrupt register- disable interrupts so set to 0x00
	//(void)writeSPIgyro(0b00100010,0b00000000);//reg 0x22-ctrl_Reg3
	//BDU BLE FS1 FS0 - 0 0 SIM
	//BDU 0=continuous update 1=not updated until both read
	//BLE 0=data LSB at lower address
	//FS 00=250dps 01=500dps 10=2000dps 11=2000dps
	//SIM-SPI serial interface 0=4wire 1=3wire
	(void)writeSPIgyro(0b00100011,0b10110000);//reg 0x23-ctrl_Reg4
	//BOOT FIFO_EN - HPen INT1 INT1 Out Out
	//BOOT 0=normal
	//FIFO 0=disable
	//Hpen high pass filter 0=disable
	//INT default 0 to disable
	//out out selection configuration default 0
	(void)writeSPIgyro(0b00100100,0b00000000);//reg 0x24-ctrl_Reg5
}
uint8_t writeSPIgyro(uint8_t regAdr, uint8_t data)//also reads the register
{
	uint8_t dummyVar;
	int32_t val;

	/* Pull CS line low */
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	SPI_I2S_SendData(SPI2, regAdr);    //Sensor Address that we are WRITING to
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
	{
		/* Wait for data */
	}
	dummyVar = SPI_I2S_ReceiveData(SPI2);

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	SPI_I2S_SendData(SPI2, data);    //Sensor Config

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
	{
		/* Wait for data */
	}
	val = (u8)SPI_I2S_ReceiveData(SPI2);

	/* Pull CS line high */
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	return (u8)val;

}
void getGyro(u8* buffer )
{
	//expressed as 2s compliment
	//u8 crtlB;
	//crtlB=(u8)writeSPIgyro(0b10100011,0x00);
	u8 gyroXL=writeSPIgyro(0b10101000,0x00);
//	u8 gyroXH=writeSPIgyro(0b10101001,0x00);
//	u8 gyroYL=writeSPIgyro(0b10101010,0x00);
//	u8 gyroYH=writeSPIgyro(0b10101011,0x00);
//	u8 gyroZL=writeSPIgyro(0b10101100,0x00);
//	u8 gyroZH=writeSPIgyro(0b10101101,0x00);
//
//	buffer[0]=gyroXL;
//	buffer[1]=gyroXH;
//	buffer[2]=gyroYL;
//	buffer[3]=gyroYH;
//	buffer[4]=gyroZL;
//	buffer[5]=gyroZH;
//	u16 t=0;
//	int i=0;
//	u8 temp=(u8)(crtlB & 0x30);
//	switch(temp)
//	{
//	case (u8)0x00://250dps
//	  for(i=0; i<3; i++)
//	  {
//		  t=(((u16)buffer[2*i] << 8) | buffer[2*i+1]);
//		  s16 temp2=twosCompToDec16(t);
//		  out[i]=(float)((temp2*8.75/1000.0));
//	  }
//	  break;
//	case (u8)0x10://500dps
//	  for(i=0; i<3; i++)
//	  {
//		  t=(((u16)buffer[2*i] << 8) |buffer[2*i+1]);
//		  s16 temp2=twosCompToDec16(t);
//		  out[i]=(float)((temp2*17.5/1000.0));
//	  }
//	  break;
//	case (u8)0x20://2000dps
//	  for(i=0; i<3; i++)
//	  {
//		  t=(((u16)buffer[2*i] << 8) |buffer[2*i+1]);
//		  s16 temp2=twosCompToDec16(t);
//		  out[i]=(float)((temp2*70/1000.0));
//	  }
//	  break;
//	case (u8)0x30://20000dps
//	  for(i=0; i<3; i++)
//	  {
//		  t=(((u16)buffer[2*i] << 8) | buffer[2*i+1]);
//		  s16 temp2=twosCompToDec16(t);
//		  out[i]=(float)((temp2*70/1000.0));
//	  }
//	  break;
//	}
}

void getTemp(u8*temp)
{
	temp[0]=(u8)writeSPIgyro(0b10100110,0x00);

//	union
//	{
//		char temp_char[2];
//		u16 temp_u16;
//	}buffer_to_char_union1;
//
//	buffer_to_char_union1.temp_u16=readADCtempValue();
//	temp[1]=buffer_to_char_union1.temp_char[0];
//	temp[2]=buffer_to_char_union1.temp_char[1];
	temp[1]=readADCtempValue();
}
s16 twosCompToDec16(u16 val)//for 16 bit
{
	u16 v=val;
	s16 temp=0;
	if((v & 0b1000000000000000)==0b1000000000000000)
	{
		temp=-32768;
	}
	if((v & 0b0000000000000001)==0b0000000000000001)
	{
		temp=temp+1;
	}
	if((v & 0b0000000000000010)==0b0000000000000010)
	{
		temp=temp+2;
	}
	if((v & 0b0000000000000100)==0b0000000000000100)
	{
		temp=temp+4;
	}
	if((v & 0b0000000000001000)==0b0000000000001000)
	{
		temp=temp+8;
	}
	if((v & 0b0000000000010000)==0b0000000000010000)
	{
		temp=temp+16;
	}
	if((v & 0b0000000000100000)==0b0000000000100000)
	{
		temp=temp+32;
	}
	if((v & 0b0000000001000000)==0b0000000001000000)
	{
		temp=temp+64;
	}
	if((v & 0b0000000010000000)==0b0000000010000000)
	{
		temp=temp+128;
	}
	if((v & 0b0000000100000000)==0b0000000100000000)
	{
		temp=temp+256;
	}
	if((v & 0b0000001000000000)==0b0000001000000000)
	{
		temp=temp+512;
	}
	if((v & 0b0000010000000000)==0b0000010000000000)
	{
		temp=temp+1024;
	}
	if((v & 0b0000100000000000)==0b0000100000000000)
	{
		temp=temp+2048;
	}
	if((v & 0b0001000000000000)==0b0001000000000000)
	{
		temp=temp+4096;
	}
	if((v & 0b0010000000000000)==0b0010000000000000)
	{
		temp=temp+8192;
	}
	if((v & 0b0100000000000000)==0b0100000000000000)
	{
		temp=temp+16384;
	}

	return (s16)temp;
}
s8 twosCompToDec8(u8 val)//for 8 bit
{
	s8 temp=0;
	if((val&0b10000000)==0b10000000)
	{
		temp=-128;
	}
	if((val&0b00000001)==0b00000001)
	{
		temp=temp+1;
	}
	if((val&0b00000010)==0b00000010)
	{
		temp=temp+2;
	}
	if((val&0b00000100)==0b00000100)
	{
		temp=temp+4;
	}
	if((val&0b00001000)==0b00001000)
	{
		temp=temp+8;
	}
	if((val&0b00010000)==0b00010000)
	{
		temp=temp+16;
	}
	if((val&0b00100000)==0b00100000)
	{
		temp=temp+32;
	}
	if((val&0b01000000)==0b01000000)
	{
		temp=temp+64;
	}
	return temp;
}












void setUpADC()
{
	V25 = 1750;// when V25=1.41V at ref 3.3V
	Avg_Slope = 5; //when avg_slope=4.3mV/C at ref 3.3V

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_TempSensorVrefintCmd(ENABLE);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1,ADC_SampleTime_41Cycles5);// ADC_SampleTime_7Cycles5);
	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));

	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
}
uint8_t readADCtempValue()
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){}
	AD_value=ADC_GetConversionValue(ADC1);
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	TemperatureC = (uint8_t)((V25-AD_value)/Avg_Slope+25);
	return TemperatureC;
}
