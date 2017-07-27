/*
 * pwm.c
 *
 *  Created on: Oct 13, 2014
 *      Author: Callen Fisher
 */
#include "pwm.h"

void setUpPWM() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //tim1 ch3 //PA10
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//set up timer

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;

	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStruct.TIM_Period = 40000; //makes it 100Hz
	TIM_TimeBaseStruct.TIM_Prescaler = 0;
	TIM_TimeBaseStruct.TIM_RepetitionCounter = 0x0000;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStruct);

	TIM_OCInitTypeDef OC;

	OC.TIM_OCMode = TIM_OCMode_PWM1;
	OC.TIM_OutputState = TIM_OutputState_Enable;
	OC.TIM_Pulse = 2000;
	OC.TIM_OCPolarity = TIM_OCPolarity_Low;

	TIM_OC3Init(TIM1, &OC);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM1, DISABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
	TIM_SetCompare3(TIM1, 10000); //inversed//set compare sets the duty (val between 0 and 40 000 where 40000 is 0% and 0 is 100%

}

void setPWM(float* valPercentage) {
	//40 000 is 0% and 0 is 100%
	if (*valPercentage > 100){
		*valPercentage = 100;	//prevent PWM going past 100% which just saturates
	}
	float valPercentage2 = 100.0 - *valPercentage;

	//valPercentage2 = valPercentage2 / 100.0 * 20000;
	valPercentage2 = 40000 - (valPercentage2 / 100.0 * 40000);
	uint16_t v = (uint16_t) valPercentage2;
	TIM_SetCompare3(TIM1, v);
}
