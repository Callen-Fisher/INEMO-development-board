/*
 * send.c
 *
 *  Created on: Jun 9, 2014
 *      Author: Callen Fisher
 */

#include "send.h"

void sendData(float*acc,float*mag,float*gyro,s8*temp)
{
	//send all data to the PC app via the XBEE using this method
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	USART_SendData(USART2,6);//just an example
	sendXBEE(acc[0]);
	sendXBEE(acc[1]);
	sendXBEE(acc[2]);
}
