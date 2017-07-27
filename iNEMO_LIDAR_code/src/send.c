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
	//USART_SendData(USART2,6);//just an example
	//USART_SendData(USART2,6);
	USART_SendData(USART2,0x7E);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);

	USART_SendData(USART2,0x0101);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	sendXBEE(acc[0]);
	//USART_SendData(USART2,6);
	sendXBEE(acc[1]);
	//USART_SendData(USART2,6);
	sendXBEE(acc[2]);
	//USART_SendData(USART2,6);
	sendXBEE(mag[0]);
	//USART_SendData(USART2,6);
	sendXBEE(mag[1]);
	//USART_SendData(USART2,6);
	sendXBEE(mag[2]);
	//USART_SendData(USART2,6);
	sendXBEE(gyro[0]);
	//USART_SendData(USART2,6);
	sendXBEE(gyro[1]);
	//USART_SendData(USART2,6);
	sendXBEE(gyro[2]);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	USART_SendData(USART2,*temp);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);



//	////////////////////////////////for single sensor acting as multiple sensors
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//	sendXBEE(acc[0]);
//	//USART_SendData(USART2,6);
//	sendXBEE(acc[1]);
//	//USART_SendData(USART2,6);
//	sendXBEE(acc[2]);
//	//USART_SendData(USART2,6);
//	sendXBEE(mag[0]);
//	//USART_SendData(USART2,6);
//	sendXBEE(mag[1]);
//	//USART_SendData(USART2,6);
//	sendXBEE(mag[2]);
//	//USART_SendData(USART2,6);
//	sendXBEE(gyro[0]);
//	//USART_SendData(USART2,6);
//	sendXBEE(gyro[1]);
//	//USART_SendData(USART2,6);
//	sendXBEE(gyro[2]);
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//	USART_SendData(USART2,*temp);
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//	sendXBEE(acc[0]);
//	//USART_SendData(USART2,6);
//	sendXBEE(acc[1]);
//	//USART_SendData(USART2,6);
//	sendXBEE(acc[2]);
//	//USART_SendData(USART2,6);
//	sendXBEE(mag[0]);
//	//USART_SendData(USART2,6);
//	sendXBEE(mag[1]);
//	//USART_SendData(USART2,6);
//	sendXBEE(mag[2]);
//	//USART_SendData(USART2,6);
//	sendXBEE(gyro[0]);
//	//USART_SendData(USART2,6);
//	sendXBEE(gyro[1]);
//	//USART_SendData(USART2,6);
//	sendXBEE(gyro[2]);
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//	USART_SendData(USART2,*temp);
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//	sendXBEE(acc[0]);
//	//USART_SendData(USART2,6);
//	sendXBEE(acc[1]);
//	//USART_SendData(USART2,6);
//	sendXBEE(acc[2]);
//	//USART_SendData(USART2,6);
//	sendXBEE(mag[0]);
//	//USART_SendData(USART2,6);
//	sendXBEE(mag[1]);
//	//USART_SendData(USART2,6);
//	sendXBEE(mag[2]);
//	//USART_SendData(USART2,6);
//	sendXBEE(gyro[0]);
//	//USART_SendData(USART2,6);
//	sendXBEE(gyro[1]);
//	//USART_SendData(USART2,6);
//	sendXBEE(gyro[2]);
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//	USART_SendData(USART2,*temp);
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
////////////////////////////////////////////////////////////////////////////////////////////
	USART_SendData(USART2,0x7E);
	//USART_SendData(USART2,10);
}
