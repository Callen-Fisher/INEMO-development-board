/*
 * USART.h
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */

#ifndef USART_H_
#define USART_H_

#include "stm32f10x.h"
#include <stddef.h>

//options for the USART:
//can be GPIOA with RX:GPIO_Pin_3 and TX:GPIO_Pin_2 and USART2
//or GPIOB with RX:GPIO_Pin_7 and TX:GPIO_Pin_6 and USART1
//or GPIOA with RX:GPIO_Pin_10 and TX:GPIO_Pin_9 and USART1
#define USARTRxPin		GPIO_Pin_3				//can be GPIO_Pin_7 or GPIO_Pin_10
#define USARTTxPin		GPIO_Pin_2				//can be GPIO_Pin_6 or GPIO_Pin_9
#define USARTPort		GPIOA					//can be GPIOB or GPIOA
#define USARTBaudRate	9600					//set the desired baud rate
#define USARTnumber		USART2					//can be USART1
#define USARTclock		RCC_APB1Periph_USART2	//can be RCC_APB1Periph_USART1

void setUpUSART(void);
void sendUSART(uint16_t data);
uint16_t receiveUSART(void);

#endif /* USART_H_ */
