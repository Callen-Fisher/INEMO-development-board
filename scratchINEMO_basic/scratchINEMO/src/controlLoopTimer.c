/*
 * controlLoopTimer.c
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */

#include "controlLoopTimer.h"

int proceed=0;
///////////////////////////////////////////////////////////////////////////
////////////////main loop timer ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void setUpLoopTimer(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//set up the interrupt handler for TIM2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);


	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;

	TIM_TimeBaseStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStruct.TIM_Period=3600-1;
	TIM_TimeBaseStruct.TIM_Prescaler=400-1;
	TIM_TimeBaseStruct.TIM_RepetitionCounter=0x0000;

	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStruct);

	TIM_Cmd(TIM2,ENABLE);

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
}
void TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    proceed=1;

  }
}
void wait(void)
{
	while(proceed==0)//interrupt will set proceed to 1;
	{

	}
	proceed=0;
}
