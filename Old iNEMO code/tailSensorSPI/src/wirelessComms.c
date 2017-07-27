/*
 * wirelessComms.c
 *
 *  Created on: Nov 25, 2014
 *      Author: Callen Fisher
 */

#include "wirelessComms.h"

void EXTI2_IRQHandler(void)//IRQ line
{
	if(EXTI_GetITStatus(EXTI_Line2))
	{
		chipSelect();
		uint8_t val=writeSPIcomms(0x07,0x07);//check the interrupt
		chipDeSelect();
		delay130micro();
		if((val&0b01000000)==0b01000000)
		{
			//data received

			//read data
			readPacket(dataRXPacket,1);
			recVal=dataRXPacket[0];

			chipDisable();
			chipSelect();
			writeSPIcomms(0x07|0b00100000,0b01110000);
			chipDeSelect();
			chipEnable();
			delay130micro();
			EXTI_ClearITPendingBit(EXTI_Line2);
		}
		else if((val&0b00100000)==0b00100000)
		{
			//transmit complete-data sent
			chipSelect();
			writeSPIcomms(0x07|0b00100000,0b01110000);
			chipDeSelect();
			delay130micro();
			EXTI_ClearITPendingBit(EXTI_Line2);
		}
		else if((val&0b00010000)==0b00010000)
		{
			//number of retries failed
			chipSelect();
			writeSPIcomms(0x07|0b00100000,0b01110000);
			chipDeSelect();
			delay130micro();
			EXTI_ClearITPendingBit(EXTI_Line2);
		}

		flushRX();
		flushTX();

		delay130micro();
//		uint8_t c=0;
//		if(dataRXPacket[0]==0x7E)
//		{
//			GPIO_SetBits(GPIOA,GPIO_Pin_1);//TURN ON THE LIGHT FOR MOTION TRACKING
//			for(c=0;c<sizeof(dataTXdata);c++)
//			{
//				dataTXdata[c]=0xB2;
//			}
//			chipDisable();
//			addAckPacket(dataTXdata,sizeof(dataTXdata));
//			chipEnable();
//		}
//		else
//		{
//			GPIO_ResetBits(GPIOA,GPIO_Pin_1);//TURN ON THE LIGHT FOR MOTION TRACKING
//			for(c=0;c<sizeof(dataTXtemp);c++)
//			{
//				dataTXtemp[c]=0xB2;
//			}
//			chipDisable();
//			addAckPacket(dataTXtemp,sizeof(dataTXtemp));
//			chipEnable();
//		}
	}
}
void addAckPacket(uint8_t* packetByte,uint8_t size)
{
	chipSelect();
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	SPI_I2S_SendData(SPI1, 0b10101000);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		/* Wait for data */
	}
	SPI_I2S_ReceiveData(SPI1);

	uint8_t c=0;
	for(c=0;c<size;c++)
	{
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		{
			/* Wait for all transmissions to complete */
		}
		SPI_I2S_SendData(SPI1, packetByte[c]);
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
		{
			/* Wait for data */
		}
		SPI_I2S_ReceiveData(SPI1);
//		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
//		{
//			/* Wait for all transmissions to complete */
//		}
	}
	chipDeSelect();
	delay130micro();
}
void initSPI()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	/* Connect SPI pins to AF5 */
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);//sck
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);//miso
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);//mosi
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	SPI_InitTypeDef  SPI_InitStructure;

	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//low
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//1 edge
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);

	//set up the IRQ and other pins
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_4;//CSN =A0, CE=A4;
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);



	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_2;//IRQ IRQ=A2
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//set up the EXTI for the IRQ pin
	EXTI_InitTypeDef   EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource2);

	setUpRegisters();
}
void setUpRegisters()
{
	chipDisable();
	delay100ms();
	//register 0x00
	//7 =0 only
	//6 =0=RX data ready interrupt
	//5 =0=TX data sent interrupt
	//4 =0=retries  interrupt
	//3 =1=enable CRC
	//2 =1=2 byte CRC
	//1 =1=power up
	//0 =1=PRX 	0=PTX (primary TX or RX)
	chipSelect();
	writeSPIcomms(0x00|0b00100000,0b00001111);
	chipDeSelect();
	delay130micro();
	delay130micro();
	//register 0x01
	//7 =0 only
	//6 =0 only
	//5 =0=disable ack pipe5
	//4 =0=disable ack pipe4
	//3 =0=disable ack pipe3
	//2 =0=disable ack pipe2
	//1 =1=enable ack pipe1
	//0 =1=enable ack pipe0
	chipSelect();
	writeSPIcomms(0x01|0b00100000,0b00000011);
	chipDeSelect();
	delay130micro();
	//register 0x02
	//7 =0 only
	//6 =0 only
	//5 =0=disable pipe5
	//4 =0=disable pipe4
	//3 =0=disable pipe3
	//2 =0=disable pipe2
	//1 =1=enable pipe1
	//0 =0=disable pipe0
	chipSelect();
	writeSPIcomms(0x02|0b00100000,0b00000011);
	chipDeSelect();
	delay130micro();
	//register 0x03
	//7 =0 only
	//6 =0 only
	//5 =0 only
	//4 =0 only
	//3 =0 only
	//2 =0 only
	//1 =1
	//0 =1 5 byte address
	chipSelect();
	writeSPIcomms(0x03|0b00100000,0b00000011);
	chipDeSelect();
	delay130micro();
	//register 0x04
	//7 =0
	//6 =1
	//5 =0
	//4 =1//auto retransmit wait 1500 us for all ACK payload sizes
	//3 =0
	//2 =0
	//1 =1
	//0 =1=3 retransmissions
	chipSelect();
	writeSPIcomms(0x04|0b00100000,0b01110011);
	chipDeSelect();
	delay130micro();
	//register 0x05
	//7 =0
	//6 =0
	//5 =0
	//4 =0
	//3 =0
	//2 =0
	//1 =1
	//0 =0 choose the frequency channel
	chipSelect();
	writeSPIcomms(0x04|0b00100000,0b00000010);
	chipDeSelect();
	delay130micro();
	//register 0x06
	//7 =0=allows continuous carrier transmit when high
	//6 =0 only
	//5 =0 only
	//4 =0 only
	//3 =1=2MBps
	//2 =1
	//1 =1=TX power of 0dBm
	//0 =1=dont care
	chipSelect();
	writeSPIcomms(0x06|0b00100000,0b00001111);
	chipDeSelect();
	delay130micro();


	//0x0A=p0 address(5 bytes)
	//0x0B=p1(5 bytes)
	//0x0C=p2(1 byte)
	//0x0D=p3(1 byte)
	//0x0E=p4(1 byte)
	//0x0F=p5(1 byte)

	//register 0x11
	//7 =0
	//6 =0
	//5 =0
	//4 =0
	//3 =0
	//2 =0
	//1 =0
	//0 =1
	chipSelect();
	writeSPIcomms(0x11|0b00100000,0b00000001);//1 byte P0
	chipDeSelect();
	delay130micro();
	//register 0x12
	//7 =0
	//6 =0
	//5 =0
	//4 =0
	//3 =0
	//2 =0
	//1 =0
	//0 =1
	chipSelect();
	writeSPIcomms(0x12|0b00100000,0b00000001);//1 byte P1
	chipDeSelect();
	delay130micro();

	//register 0x1C
	//7 =0 only
	//6 =0 only
	//5 =0
	//4 =0
	//3 =1 enable p3 dynamic payload length
	//2 =1 enable p2 dynamic payload length
	//1 =1 enable p1 dynamic payload length
	//0 =1 enable p0 dynamic payload length
	chipSelect();
	writeSPIcomms(0x1C|0b00100000,0b00000011);
	chipDeSelect();
	delay130micro();
	//register 0x1D
	//7 =0 only
	//6 =0 only
	//5 =0 only
	//4 =0 only
	//3 =0 only
	//2 =1 enable dynamic payload length
	//1 =1 enable payload with ACK
	//0 =0 disable the TX with no ACK command
	chipSelect();
	writeSPIcomms(0x1D|0b00100000,0b00000110);
	chipDeSelect();
	delay130micro();

	setADDR(0x10,0xB3,0xB4,0xB5,0xB6,SENSORADDRESS);
	setADDR(0x0A,0xB3,0xB4,0xB5,0xB6,SENSORADDRESS);
	delay130micro();
	setADDRshort(0x0B,SENSORADDRESS);
}
void chipEnable()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}
void chipDisable()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}
void chipSelect()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}
void chipDeSelect()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}
void delay100ms()
{
	uint8_t p=0;
	uint8_t q=0;
	uint8_t r=0;
	uint8_t s=0;
	for(s=0;s<2;s++)
	{
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
		for( p=0 ; p<254 ; p++)
		{
			for(r=0;r<254;r++)
			{
				q=p;
			}
		}
	}
}
void delay130micro()
{
	uint8_t p=0;
	uint8_t q=0;
	uint8_t r=0;
	uint8_t s=0;
	for(s=0;s<13;s++)
	{
	for( p=0 ; p<10 ; p++)
	{
		for(r=0;r<10;r++)
		{
			q=p;
		}
	}
	}
}
void delay10micro()
{
	uint8_t p=0;
	uint8_t q=0;
	uint8_t r=0;
	for( p=0 ; p<10 ; p++)
	{
		for(r=0;r<11;r++)
		{
			q=p;
		}
	}
}
uint8_t writeSPIcomms(uint8_t regAdr, uint8_t data)
{
	uint8_t dummyVar;
	int32_t val;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	SPI_I2S_SendData(SPI1, regAdr);    //Sensor Address that we are WRITING to
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
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
	val = (u8)SPI_I2S_ReceiveData(SPI1);

	return (u8)val;
}
void setADDR(uint8_t Pipe, uint8_t one, uint8_t two, uint8_t three, uint8_t four, uint8_t five)
{
	delay130micro();
	chipSelect();
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	SPI_I2S_SendData(SPI1, Pipe|0b00100000);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		/* Wait for data */
	}
	SPI_I2S_ReceiveData(SPI1);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	SPI_I2S_SendData(SPI1, one);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		/* Wait for data */
	}
	SPI_I2S_ReceiveData(SPI1);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	SPI_I2S_SendData(SPI1, two);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		/* Wait for data */
	}
	SPI_I2S_ReceiveData(SPI1);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	SPI_I2S_SendData(SPI1, three);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		/* Wait for data */
	}
	SPI_I2S_ReceiveData(SPI1);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	SPI_I2S_SendData(SPI1, four);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		/* Wait for data */
	}
	SPI_I2S_ReceiveData(SPI1);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	SPI_I2S_SendData(SPI1, five);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		/* Wait for data */
	}
	SPI_I2S_ReceiveData(SPI1);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	chipDeSelect();
	delay130micro();
}
void setADDRshort(uint8_t Pipe, uint8_t one)
{
	delay130micro();
	chipSelect();
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	SPI_I2S_SendData(SPI1, Pipe|0b00100000);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		/* Wait for data */
	}
	SPI_I2S_ReceiveData(SPI1);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	SPI_I2S_SendData(SPI1, one);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		/* Wait for data */
	}
	SPI_I2S_ReceiveData(SPI1);
	chipDeSelect();
	delay130micro();
}
void readPacket(uint8_t* data,uint8_t length)
{
	chipDisable();
	chipSelect();
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	SPI_I2S_SendData(SPI1, 0x61);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	SPI_I2S_ReceiveData(SPI1);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	uint8_t q;
	for(q=0;q<length;q++)
	{
		SPI_I2S_SendData(SPI1, 0x00);    //Sensor Address that we are WRITING to
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
		{
			/* Wait for data */
		}

	    data[q]=SPI_I2S_ReceiveData(SPI1);
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		{
			/* Wait for all transmissions to complete */
		}
	}
	chipDeSelect();
	delay130micro();
	chipEnable();
}
void flushTX()
{
	delay130micro();
	chipSelect();
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	SPI_I2S_SendData(SPI1, 0xE1);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		/* Wait for data */
	}
	SPI_I2S_ReceiveData(SPI1);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	chipDeSelect();
	delay130micro();
}
void flushRX()
{
	delay130micro();
	chipSelect();
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	SPI_I2S_SendData(SPI1, 0xE2);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		/* Wait for data */
	}
	SPI_I2S_ReceiveData(SPI1);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		/* Wait for all transmissions to complete */
	}
	chipDeSelect();
	delay130micro();
}
