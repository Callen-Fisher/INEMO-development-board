/*
 * XBEE.c
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */
#include "XBEE.h"

///////////////////////////////////////////////////////////////////////////
/////////////XBEE//////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void setUpXbee(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Pack the struct */
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate=230400;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;

	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

	//Configure DMA TX Stream interrupt in the NVIC
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART2,DISABLE);
	S_DMA();


}
void S_DMA()
{

	//DMA_DeInit(DMA1_Channel7);

		DMA_InitTypeDef DMA_InitStructure;
		DMA_DeInit(DMA1_Channel7);
		DMA_StructInit(&DMA_InitStructure);
		//DMA_InitStructure.DMA_Channel = DMA_Channel_4;
		DMA_InitStructure.DMA_PeripheralBaseAddr =  (USART2_BASE+0x04); //USART Data Register
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) TxBuff;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//DMA_DIR_PeripheralSRC;//;//
		DMA_InitStructure.DMA_BufferSize = sizeof(TxBuff);
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

		DMA_Init(DMA1_Channel7, &DMA_InitStructure);
		DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);

		DMA_Cmd(DMA1_Channel7,ENABLE);
		USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
		USART_Cmd(USART2,ENABLE);

		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
		while(DMA_GetFlagStatus(DMA1_FLAG_TC7)==RESET);

		DMA_ClearFlag(DMA1_FLAG_TC7);
		USART_ClearFlag(USART2,USART_FLAG_TC);

}
void sendXBEE(float Data)
{
//	u16 byte1=(u8)((int)Data & 0x 00 00 00 FF);
//	u16 byte2=(u8)(((int)Data>>8) & 0x000000FF);
//	u16 byte3=(u8)(((int)Data>>16) & 0x000000FF);
//	u16 byte4=(u8)(((int)Data>>24) & 0x000000FF);

	//A union is a special data type available in C that enables you to store different
	//data types in the same memory location. You can define a union with many members,
	//but only one member can contain a value at any given time. Unions provide an efficient
	//way of using the same memory location for multi-purpose.

	union// a union is a value that may have any of several representations or formats
	{
		char temp_char[4];//4 bytes in a float
		float temp_float_buffer;//1 float value
	}buffer_to_char_union;//both the 4 chars and the float have the same value just in a different format

	buffer_to_char_union.temp_float_buffer=Data;//assign a value to the float

	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	USART_SendData(USART2,buffer_to_char_union.temp_char[0]);//send the first char
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	USART_SendData(USART2,buffer_to_char_union.temp_char[1]);//send the second char
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	USART_SendData(USART2,buffer_to_char_union.temp_char[2]);//send the third char
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	USART_SendData(USART2,buffer_to_char_union.temp_char[3]);//send the fourth char
}
uint16_t receiveXBEE(void)
{
	uint16_t data=USART_ReceiveData(USART2);
	return data;
}


