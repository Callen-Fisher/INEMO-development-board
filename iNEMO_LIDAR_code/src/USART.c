/*
 * USART.c
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */

//to set up the USART port change the #define variable in the USART.h file
//if you require to set up more then 1 USART then copy and edit the functions
//as necessary
//make sure to call the setUpUSART() method in the main method.
#include "USART.h"

void setUpLoggerSDcard(void) {
	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/* Enable USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//DMA 1 clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);


	GPIO_InitTypeDef GPIO_InitStruct;
	/* Configure USART Rx & Tx as alternate function */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	USART_InitTypeDef USART_InitStructure;

	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

	USART_InitStructure.USART_BaudRate = 115200; //115200;	//SD card can go faster than Xbee
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;

	USART_Init(USART1, &USART_InitStructure);

	setUpDMA1channel4();
	USART_Cmd(USART1, ENABLE);
}

void setUpDMA1channel4() {
	DMA_InitTypeDef DMA_InitStructure;
	//DMA_DeInit(DMA1_Channel7);
	DMA_StructInit(&DMA_InitStructure);
	//DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (USART1_BASE + 0x04); //USART Data Register
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) TxBuff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; //DMA_DIR_PeripheralSRC;//;//
	DMA_InitStructure.DMA_BufferSize = TX_BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel4, &DMA_InitStructure);

	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	DMA_Cmd(DMA1_Channel4, ENABLE);
}

void setUpUSART(void) {
	RCC_APB1PeriphClockCmd(USARTclock, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Pack the struct */
	GPIO_InitStruct.GPIO_Pin = USARTTxPin | USARTRxPin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USARTPort, &GPIO_InitStruct);

	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = USARTBaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;

	USART_Init(USARTnumber, &USART_InitStructure);

	USART_Cmd(USARTnumber, ENABLE);
}

void sendUSART(uint16_t Data) {
	u16 low = (u8) (Data & 0x00FF);
	u16 high = (u8) ((Data >> 8) & 0x00FF);
	while (USART_GetFlagStatus(USARTnumber, USART_FLAG_TXE) == RESET)
		;
	USART_SendData(USARTnumber, low);
	while (USART_GetFlagStatus(USARTnumber, USART_FLAG_TXE) == RESET)
		;
	USART_SendData(USARTnumber, high);
}
uint16_t receiveUSART(void) {
	uint16_t data = USART_ReceiveData(USARTnumber);
	return data;
}
