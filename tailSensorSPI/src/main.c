
#include <stddef.h>
#include "stm32f10x.h"
#include "AccMag.h"
#include "gyro.h"
#include "XBEE.h"
#include "serial_terminal.h"
#include "CRC.h"
#include "controlLoopTimer.h"
#include "wirelessComms.h"
#include "main.h"


uint8_t responseCode=0xC3;



int main(void)
{
	delay100ms();
	delay100ms();
	delay100ms();
	delay100ms();
	delay100ms();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	//GPIO_SetBits(GPIOA,GPIO_Pin_1);//TURN ON THE LIGHT FOR MOTION TRACKING

	serialTerminal_Init();
	setUpEcompass();
	setUpGyro();
	initSPI();
	delay100ms();

	setUpADC();

	chipEnable();
	delay130micro();
	EXTI_ClearITPendingBit(EXTI_Line2);

	//flush TX and RX
	chipDisable();
	flushRX();
	flushTX();


	recVal=1;


	GPIO_SetBits(GPIOB,GPIO_Pin_6);
		  GPIO_SetBits(GPIOB,GPIO_Pin_7);
		  GPIO_SetBits(GPIOB,GPIO_Pin_9);
	chipEnable();
  while (1)
  {

	  if(recVal==0x7E)
	  {
		  recVal=0;
		 // GPIO_SetBits(GPIOA,GPIO_Pin_1);
		  getAcc(acc8);
		  getMag(mag8);
		  getGyro(gyro8);
		  getTemp(temp8);
		  uint8_t c;
		  for(c=0;c<sizeof(gyro8);c++)
		  {
		  	dataTXdata[c]=gyro8[c];
		  }
		  for(c=0;c<sizeof(mag8);c++)
		  {
		  	dataTXdata[c+sizeof(gyro8)]=mag8[c];
		  }
		  for(c=0;c<sizeof(acc8);c++)
		  {
		  	dataTXdata[c+sizeof(gyro8)+sizeof(mag8)]=acc8[c];
		  }
		  for(c=0;c<sizeof(temp8);c++)
		  {
		  	dataTXdata[c+sizeof(gyro8)+sizeof(mag8)+sizeof(acc8)]=temp8[c];
		  }

		  chipDisable();
		  addAckPacket(dataTXdata,sizeof(dataTXdata));
		  chipEnable();

		  while(recVal!=responseCode);
		  //GPIO_ResetBits(GPIOA,GPIO_Pin_1);
		  recVal=0;

		  for(c=0;c<sizeof(dataTXtemp);c++)
		  {
			  dataTXtemp[c]=temp8[c];
		  }
		  chipDisable();
		  addAckPacket(dataTXtemp,sizeof(dataTXtemp));
		  chipEnable();

		  //clear the buffer
		  for(p=0;p<sizeof(dataTXdata);p++)
		  {
		  	dataTXdata[p]=0;
		  }
		  for(p=0;p<sizeof(dataTXtemp);p++)
		  {
			  dataTXtemp[p]=0;
		  }
	  }
  }
}
//void convertFloatToByteArray()
//{
//	union
//	{
//		char temp_char[12];
//		float temp_float_buffer[3];
//	}buffer_to_char_union1;
//	buffer_to_char_union1.temp_float_buffer[0]=acc[0];
//	buffer_to_char_union1.temp_float_buffer[1]=acc[1];
//	buffer_to_char_union1.temp_float_buffer[2]=acc[2];
//	int i;
//	for(i=0;i<12;i++)
//	{
//		acc8[i]=buffer_to_char_union1.temp_char[i];
//	}
//
//	union
//	{
//		char temp_char[12];
//		float temp_float_buffer[3];
//	}buffer_to_char_union2;
//	buffer_to_char_union2.temp_float_buffer[0]=mag[0];
//	buffer_to_char_union2.temp_float_buffer[1]=mag[1];
//	buffer_to_char_union2.temp_float_buffer[2]=mag[2];
//	for(i=0;i<12;i++)
//	{
//		mag8[i]=buffer_to_char_union2.temp_char[i];
//	}
//	union
//	{
//		char temp_char[12];
//		float temp_float_buffer[3];
//	}buffer_to_char_union3;
//	buffer_to_char_union3.temp_float_buffer[0]=gyro[0];
//	buffer_to_char_union3.temp_float_buffer[1]=gyro[1];
//	buffer_to_char_union3.temp_float_buffer[2]=gyro[2];
//	for(i=0;i<12;i++)
//	{
//		gyro8[i]=buffer_to_char_union3.temp_char[i];
//	}
//	union
//	{
//		s8 temp_signed;
//		uint8_t temp_unsigned;
//	}buffer_to_char_union4;
//	buffer_to_char_union4.temp_signed=temp;
//	temp8=buffer_to_char_union4.temp_unsigned;
//}
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}
