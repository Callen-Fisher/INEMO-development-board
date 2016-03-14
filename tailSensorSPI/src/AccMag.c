/*
 * AccMag.c
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */

#include "AccMag.h"

///////////////////////////////////////////////////////////////////////////
///////////////ACC AND MAGN FUNCTIONS//////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void setUpEcompass()
{
	initI2CEcompass();
	initRegistersEcompass();
}
void initI2CEcompass(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable I2C and GPIO clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);

	/* Configure I2C pins: SCL and SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* I2C configuration */
	I2C_DeInit(I2C2);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;

	/* Apply I2C configuration after enabling it */
	I2C_Init(I2C2, &I2C_InitStructure);

	/* I2C Peripheral Enable */
	I2C_Cmd(I2C2, ENABLE);
}
void initRegistersEcompass(void)
{
	//the slave address-0b001100xy->0x33=read  0x32=write
	//lsm303dlh

	//register address 0x20-power mode, odr,axes->0b001 10 111
	//ctrl_reg1_A
	//PM2 PM1 PM0 DR1 DR0 Zen Yen Xen
	//PM2-PM0 000(power down) 001(normal mode) other = low power mde
	//Dr1-Dro data rate- 00=50hz 01=100hz 10=400hz 11=1000hz
	//Zen, Yen, Xen 1=enable 0=disable
	while((I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY)));
	I2C_GenerateSTART(I2C2,ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C2,0x32,I2C_Direction_Transmitter);//slave address    33
	//I2C_SendData(I2C2,(u8)0x32);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C2,(u8)0x20);//write address
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2C2,0b00111111);//data-0x27
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(I2C2,ENABLE);

	//register address 0x23-Fs, data update,endianess->0b1 0 11 0 0 0 0
	//BDU BLE FS1 FS0 STsign 0 ST -
	//BDU block data update 1= output registers not updated between MSB and LSB reading
	//BLE (BIG LITTLE endian) 0=data LSB at lower address 1= data MSB at lower address
	//FS1,FS0 00=+-2g 01=+-4g 11=+-8g?? ERROR HERE
	//STsign self test sign 0=self test plus, 1= self test minus (default=0)
	//ST= self test enable 0=disable, 1= enable
	while((I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY)));
	I2C_GenerateSTART(I2C2,ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C2,(u8)0x33,I2C_Direction_Transmitter);//slave address
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C2,(u8)0x23);//write address
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2C2,(u8)0b10010000);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(I2C2,ENABLE);





	int z;
	int y=0;
	for(z=0;z<10000;z++)
	{
		y=y+1;
	}





	///////////////The magnatometer registers
	//write 3c read 3d

	//register 0x00-> register A->0b000 110 00->data rate and config
	//0 0 0 DO2 DO1 DO0 MS1 MS0
	//DO2 to DO0 data output rate bits
	//000  =0.75hz
	//001  =1.5hz
	//010  =3hz
	//011  =7.5hz
	//100  =15hz
	//101  =30hz
	//110  =75hz
	//111  =not used
	//MS1 to MSo measurement config bits
	//00=normal measurement (default)
	//01=positive bias config
	//10=negative bias config
	//11=not used
	while((I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY)));
	I2C_GenerateSTART(I2C2,ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));
	//I2C_Send7bitAddress(I2C2,(u8)0x3C,I2C_Direction_Transmitter);//slave address
	I2C_SendData(I2C2,(u8)0x3C);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C2,(u8)0x00);//write address
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2C2,(u8)0b00011000);//data
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(I2C2,ENABLE);

	//register 0x01->0b11100000->mag gain-+ 4Ga
	//GN2 GN1 GN0 0 0 0 0 0
	//001 =+-1.3
	//010 =+-1.9
	//011 =+-2.5
	//100 =+-4
	//101 =+-4.7
	//110 =+-5.6
	//111 =+-8.1
	while((I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY)));
	I2C_GenerateSTART(I2C2,ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C2,(u8)0x3C,I2C_Direction_Transmitter);//slave address
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C2,(u8)0x01);//write address
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2C2,(u8)0b11100000);//data
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(I2C2,ENABLE);

	//register 0x02->0b00000000-> single conversion mode
	//0 0 0 0 0 0 MD1 MD0
	//00= continuous mode
	//01= single conversion mode
	//11=sleep mode
	while((I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY)));
	I2C_GenerateSTART(I2C2,ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));
	//I2C_Send7bitAddress(I2C2,(u8)0x3C,I2C_Direction_Transmitter);//slave address
	I2C_SendData(I2C2,(u8)0x3C);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C2,(u8)0x02);//write address
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2C2,0x00);//data
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(I2C2,ENABLE);
}
void eCompassRead(u8 address,uint32_t numByteToRead,u8* pBuffer)//for the acc
{
	while((I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY)));
	I2C_GenerateSTART(I2C2,ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C2,(u8)0x32,I2C_Direction_Transmitter);
	//I2C_SendData(I2C2,0x32);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_Cmd(I2C2,ENABLE);
	I2C_SendData(I2C2,(u8)address);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTART(I2C2,ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C2,(u8)0x33,I2C_Direction_Receiver);
	//I2C_SendData(I2C2,0x33);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	while(numByteToRead)
	{
		if(numByteToRead==1)
     	{
			I2C_AcknowledgeConfig(I2C2,DISABLE);
			I2C_GenerateSTOP(I2C2,ENABLE);
		}
		if(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED))

		{
			*pBuffer=I2C_ReceiveData(I2C2);
			numByteToRead--;
		}

	}
	I2C_AcknowledgeConfig(I2C2,ENABLE);
}
void eCompassRead2(u8 address,uint32_t numByteToRead,u8* pBuffer)//magnatometer
{
	while((I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY)));
	I2C_GenerateSTART(I2C2,ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));
	//I2C_Send7bitAddress(I2C2,(u8)0x3C,I2C_Direction_Transmitter);
	I2C_SendData(I2C2,0x3C);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_Cmd(I2C2,ENABLE);
	I2C_SendData(I2C2,(u8)address);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTART(I2C2,ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));
	//I2C_Send7bitAddress(I2C2,(u8)0x3D,I2C_Direction_Receiver);
	I2C_SendData(I2C2,0x3D);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	while(numByteToRead)
	{
		if(numByteToRead==1)
		{
			I2C_AcknowledgeConfig(I2C2,DISABLE);
			I2C_GenerateSTOP(I2C2,ENABLE);
		}
		if(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			*pBuffer=I2C_ReceiveData(I2C2);
			numByteToRead--;
		}
	}
	I2C_AcknowledgeConfig(I2C2,ENABLE);
}
void eCompassWrite(u8 data, u8 address)
{
	while((I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY)));
	I2C_GenerateSTART(I2C2,ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C2,(u8)0x32,I2C_Direction_Transmitter);//slave address
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C2,(u8)address);//write address
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2C2,(u8)data);//data
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(I2C2,ENABLE);
}
void getAcc(u8* buffer)
{
	//u8 buffer[6];
	u8 crtl4;

	eCompassRead(0x23,1,&crtl4);
	eCompassRead(0x28,1,&buffer[0]);
	eCompassRead(0x29,1,&buffer[1]);
	eCompassRead(0x2A,1,&buffer[2]);
	eCompassRead(0x2B,1,&buffer[3]);
	eCompassRead(0x2C,1,&buffer[4]);
	eCompassRead(0x2D,1,&buffer[5]);
//
//	int i=0;
//	u16 t=0;
//
//	if(!(crtl4 & 0x40))//endian check
//	{
//		switch(crtl4 & 0x30)//adjust the sensitivity
//		{
//		case 0x00:
//			for(i=0;i<3;i++)
//			{
//				s16 te=0;
//				t=(u16)(((u16)buffer[2*i+1]<<8)|(u16)buffer[2*i]);
//				te=twosCompToDec(t);
//				//out[i]=(float)(((te/16)*9.81/1000.0));
//				out[i]=(float)(((te)/16/1000.0));
//			}
//			break;
//		case 0x10:
//			for(i=0;i<3;i++)
//			{
//				s16 te=0;
//				t=(u16)(((u16)buffer[2*i+1]<<8)|(u16)buffer[2*i]);
//				te=twosCompToDec(t);
//				out[i]=(float)((te/(16*2))*9.81/1000.0);
//			}
//			break;
//		case 0x30:
//			for(i=0;i<3;i++)
//			{
//				s16 te=0;
//				t=(u16)(((u16)buffer[2*i+1]<<8)|(u16)buffer[2*i]);
//				te=twosCompToDec(t);
//				out[i]=(float)(5*(te/(16*3.9))*9.81/1000.0);
//			}
//			break;
//		}
//	}
}
void getMag(u8* buffer)
{
	u8 crtlB;
	int i=0;
	eCompassRead2(0x01,1,&crtlB);
	eCompassRead2(0x03,1,&buffer[1]);
	eCompassRead2(0x04,1,&buffer[0]);
	eCompassRead2(0x05,1,&buffer[3]);
	eCompassRead2(0x06,1,&buffer[2]);
	eCompassRead2(0x07,1,&buffer[5]);
	eCompassRead2(0x08,1,&buffer[4]);
//	s16 te=0;
//	u16 t=0;
//	/** switch the sensitivity set in the CRTLB*/
//    switch(crtlB & 0xE0)
//    {
//    case 0x40:
//	  for(i=0; i<2; i++)
//	  {
//		  t=((u16)buffer[2*i] << 8) | (u16)buffer[2*i+1];
//		  te=twosCompToDec(t);
//		  out[i]=(float)((te)/1055);
//	  }
//	  t=((u16)buffer[4] << 8) | (u16)buffer[5];
//	  te=twosCompToDec(t);
//	  out[2]=(float)(te/950);
//	  break;
//    case 0x60:
//	  for(i=0; i<2; i++)
//	  {
//		  t=((u16)buffer[2*i] << 8) |(u16) buffer[2*i+1];
//		  te=twosCompToDec(t);
//		  out[i]=(float)(te/795);
//	  }
//	  t=((u16)buffer[4] << 8) | (u16)buffer[5];
//	  te=twosCompToDec(t);
//	  out[2]=(float)(te/710);
//	  break;
//    case 0x80:
//	  for(i=0; i<2; i++)
//	  {
//		  t=((u16)buffer[2*i] << 8) | (u16)buffer[2*i+1];
//		  te=twosCompToDec(t);
//		  out[i]=(float)(te/635);
//	  }
//	  t=((u16)buffer[4] << 8) | (u16)buffer[5];
//	  te=twosCompToDec(t);
//	  out[2]=(float)(te/570);
//	  break;
//    case 0xA0:
//	  for(i=0; i<2; i++)
//	  {
//		  t=((u16)buffer[2*i] << 8) | (u16)buffer[2*i+1];
//		  te=twosCompToDec(t);
//		  out[i]=(float)(te/430);
//	  }
//	  t=((u16)buffer[4] << 8) | (u16)buffer[5];
//	  te=twosCompToDec(t);
//	  out[2]=(float)(te/385);
//	  break;
//    case 0xB0:
//	  for(i=0; i<2; i++)
//	  {
//		  t=((u16)buffer[2*i] << 8) | (u16)buffer[2*i+1];
//		  te=twosCompToDec(t);
//		  out[i]=(float)(te/375);
//	  }
//	  t=((u16)buffer[4] << 8) | (u16)buffer[5];
//	  te=twosCompToDec(t);
//	  out[2]=(float)(te/335);
//	  break;
//    case 0xC0:
//	  for(i=0; i<2; i++)
//	  {
//		  t=((u16)buffer[2*i] << 8) |(u16) buffer[2*i+1];
//		  te=twosCompToDec(t);
//		  out[i]=(float)(te/320);
//	  }
//	  t=((u16)buffer[4] << 8) | (u16)buffer[5];
//	  te=twosCompToDec(t);
//	  out[2]=(float)(te/285);
//	  break;
//    case 0xE0:
//	  for(i=0; i<2; i++)
//	  {
//		  t=(((u16)buffer[2*i] << 8) | (u16)buffer[2*i+1]);
//		  te=twosCompToDec(t);
//		  out[i]=(float)(te/230);
//	  }
//	  t=((u16)buffer[4] << 8) | (u16)buffer[5];
//	  te=twosCompToDec(t);
//	  out[2]=(float)(te/205);
//	  break;
//    }
}
s16 twosCompToDec(u16 val)//for 16 bit
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

