/*
 * main.c
 *
 *  Created on: May 25, 2014
 *      Author: Callen Fisher
 */
//Include all the relevant (header) files
//#include <stddef.h>
#include "stm32f10x.h"
#include "AccMag.h"
#include "controlLoopTimer.h"
#include "gpioPorts.h"
#include "gyro.h"
#include "testPorts.h"
#include "XBEE.h"
#include "DMA.h"
#include "ADC.h"
#include "control.h"
#include "send.h"
#include "serial_terminal.h"
#include "CRC.h"
uint32_t temp=0;
uint16_t recVal;
int main(void)
{
  serialTerminal_Init();
  setUpLoopTimer();
  //setUpGPIO();

  pwm();
  while(1)
  {

  }
  setUpEcompass();
  setUpGyro();
  setUpXbee();

  float acc[3];
  float mag[3];
  float gyro[3];
  s8 temp;

  uint8_t acc8[12];
  uint8_t mag8[12];
  uint8_t gyro8[12];
  uint8_t temp8;
  USART_Cmd(USART2,ENABLE);
  while (1)
  {
	  recVal=0;
	  recVal=USART_ReceiveData(USART2);
	  if(recVal==0x7E&&USART_GetFlagStatus(USART2,USART_IT_RXNE))
	  {
		  USART_ClearFlag(USART2,USART_IT_RXNE);
		  TIM_SetCounter(TIM2,0x00);//TODO: This could force an interrupt, not sure, double check

	  USART_Cmd(USART2,DISABLE);
	  DMA_DeInit(DMA1_Channel7);
	  controlMethod(&acc[0],&mag[0],&gyro[0],&temp);//get the data


	  union
	  {
	  	char temp_char[12];
	  	float temp_float_buffer[3];
	  }buffer_to_char_union1;
	  buffer_to_char_union1.temp_float_buffer[0]=acc[0];
	  buffer_to_char_union1.temp_float_buffer[1]=acc[1];
	  buffer_to_char_union1.temp_float_buffer[2]=acc[2];
	  int i;
	  for(i=0;i<12;i++)
	  {
		  acc8[i]=buffer_to_char_union1.temp_char[i];
	  }

	  union
	  {
	   	char temp_char[12];
	   	float temp_float_buffer[3];
	  }buffer_to_char_union2;
	  buffer_to_char_union2.temp_float_buffer[0]=mag[0];
	  buffer_to_char_union2.temp_float_buffer[1]=mag[1];
	  buffer_to_char_union2.temp_float_buffer[2]=mag[2];
	  for(i=0;i<12;i++)
	  {
		  mag8[i]=buffer_to_char_union2.temp_char[i];
	  }
	  union
	  {
	   	char temp_char[12];
	   	float temp_float_buffer[3];
	  }buffer_to_char_union3;
	  buffer_to_char_union3.temp_float_buffer[0]=gyro[0];
	  buffer_to_char_union3.temp_float_buffer[1]=gyro[1];
	  buffer_to_char_union3.temp_float_buffer[2]=gyro[2];
	  for(i=0;i<12;i++)
	  {
		  gyro8[i]=buffer_to_char_union3.temp_char[i];
	  }
	  union
	  {
	   	s8 temp_signed;
	   	uint8_t temp_unsigned;
	  }buffer_to_char_union4;
	  buffer_to_char_union4.temp_signed=temp;
	  temp8=buffer_to_char_union4.temp_unsigned;

	  serialTerminal_packetize(gyro8,acc8,mag8,&temp8,12,12,12,1);
	  USART_Cmd(USART2,ENABLE);
	  S_DMA();//re init the DMA and then it will send

	  wait();
	  wait();
	  }
  }
}
///////////////////////////////////////////////////////////////////////////
/////don't delete the following functions//////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}
