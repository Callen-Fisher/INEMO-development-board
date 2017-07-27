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
#include "serial_terminal.h"

//options for the USART:
//can be GPIOA with RX:GPIO_Pin_3 and TX:GPIO_Pin_2 and USART2
//or GPIOB with RX:GPIO_Pin_7 and TX:GPIO_Pin_6 and USART1
//or GPIOA with RX:GPIO_Pin_10 and TX:GPIO_Pin_9 and USART1
#define USARTRxPin		GPIO_Pin_3				//can be GPIO_Pin_7 or GPIO_Pin_10
void setUpLoggerSDcard(void);
void setUpDMA1channel4();
void setUpUSART(void);
void sendUSART(uint16_t data);
uint16_t receiveUSART(void);

#endif /* USART_H_ */