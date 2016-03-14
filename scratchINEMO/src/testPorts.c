/*
 * testPorts.c
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */
#include "testPorts.h"


///////////test functions
void test(void)
{
	initGPIO();

	while(1)
	{
		flash();
	}
}
void initGPIO(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9 | GPIO_Pin_12|GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void flash(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_9);
	del();
	GPIO_SetBits(GPIOA,GPIO_Pin_10);
	del();
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
	del();
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
	del();
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	del();
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
	del();
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	del();
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	del();
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	del();
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	del();
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
	del();
	GPIO_ResetBits(GPIOA,GPIO_Pin_9);
	del();
	GPIO_ResetBits(GPIOA,GPIO_Pin_10);
	del();
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	del();
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	del();
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	del();
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	del();
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	del();
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	del();
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	del();
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	del();
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	del();
}
void del(void)
{
	int i=0;
	int j=0;

	for(i=0;i<1000;i++)
	{
		for(j=0;j<500;j++)
		{

		}
	}
}


