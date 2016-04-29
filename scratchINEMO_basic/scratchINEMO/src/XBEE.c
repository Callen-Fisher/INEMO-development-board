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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Pack the struct */
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;

	USART_Init(USART2, &USART_InitStructure);

	USART_Cmd(USART2,ENABLE);
}
void sendXBEE(float Data)
{
//	u16 byte1=(u8)((int)Data & 0x000000FF);
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


