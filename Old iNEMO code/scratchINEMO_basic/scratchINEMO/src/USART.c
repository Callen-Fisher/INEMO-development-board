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

void setUpUSART(void)
{
	RCC_APB1PeriphClockCmd(USARTclock,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Pack the struct */
	GPIO_InitStruct.GPIO_Pin =USARTTxPin|USARTRxPin;
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(USARTPort, &GPIO_InitStruct);

	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate=USARTBaudRate;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;

	USART_Init(USARTnumber, &USART_InitStructure);

	USART_Cmd(USARTnumber,ENABLE);
}

void sendUSART(uint16_t Data)
{
	u16 low=(u8)(Data & 0x00FF);
	u16 high=(u8)((Data>>8)&0x00FF);
	while(USART_GetFlagStatus(USARTnumber,USART_FLAG_TXE)==RESET);
	USART_SendData(USARTnumber,low);
	while(USART_GetFlagStatus(USARTnumber,USART_FLAG_TXE)==RESET);
	USART_SendData(USARTnumber,high);
}
uint16_t receiveUSART(void)
{
	uint16_t data=USART_ReceiveData(USARTnumber);
	return data;
}
