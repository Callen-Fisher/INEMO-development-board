/*
 * controlLoopTimer.c
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */

#include "controlLoopTimer.h"

int proceed10ms = 0;
///////////////////////////////////////////////////////////////////////////
////////////////main loop timer ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void setUpLoopTimer(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //set up the interrupt handler for TIM2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;

	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStruct.TIM_Period = (7200 / 4 - 1); //makes it 100Hz - confirmed on scope.
//	TIM_TimeBaseStruct.TIM_Period = (3600 / 4 - 1); //experimenting, 200Hz
	//TIM_TimeBaseStruct.TIM_Period = (2888 / 4 - 1); //experimenting, 250Hz
	//TIM_TimeBaseStruct.TIM_Period = (1440 / 4 - 1); //experimenting, 500Hz
	TIM_TimeBaseStruct.TIM_Prescaler = 400 - 1;
	TIM_TimeBaseStruct.TIM_RepetitionCounter = 0x0000;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);

	TIM_Cmd(TIM2, ENABLE);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

}

void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		proceed10ms = 1;
	}
}

void setUpEncoder(void) {
//http://www.micromouseonline.com/2013/02/16/quadrature-encoders-with-the-stm32f4/
//https://my.st.com/public/STe2ecommunities/mcu/Lists/cortex_mx_stm32/Flat.aspx?RootFolder=https%3a%2f%2fmy%2est%2ecom%2fpublic%2fSTe2ecommunities%2fmcu%2fLists%2fcortex_mx_stm32%2fSTM32F107%20Quadrature%20encoder%20example&FolderCTID=0x01200200770978C69A1141439FE559EB459D7580009C4E14902C3CDE46A77F0FFD06506F5B&currentviews=5599
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_Period = 65535; // Maximal
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	// TIM_EncoderMode_TI1: Counter counts on TI1FP1 edge depending on TI2FP2 level.
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising,
			TIM_ICPolarity_Rising);

	TIM_Cmd(TIM3, ENABLE);
}

void waitForEndTimerHz(void) {
	while (proceed10ms == 0) //interrupt will set proceed to 1;
	{

	}
	proceed10ms = 0;
}
