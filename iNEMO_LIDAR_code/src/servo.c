/*
 * servo.c
 *
 *  Created on: 28 Jul 2017
 *      Author: Sylvan Morris
 */

//Set up for a 20ms servo with a mid at 1.5ms and range 0.9 - 2.1 ms.
// Assuming they turn clockwise
#include "servo.h"
#define MAX 2.1
#define MIN 0.9

void setUpServo() {
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
	TIM_TimeBaseStruct.TIM_Period = 14399; //makes it 50Hz
	TIM_TimeBaseStruct.TIM_Prescaler = 99; // Needed since the ARR is restricted to less than 0x77
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


void setServo(float* angle) {
	//80 000 is 0% and 0 is 100%
	if (*angle > 90){
		*angle = 90;	// servo limit
	}
	else if (*angle < -90){
		*angle = -90;	// servo limit
	}
	float angle2 = 14399 - ((*angle/150 + 1.5)/20) *14399;

	uint16_t v = (uint16_t) angle2;
	TIM_SetCompare3(TIM1, v);
}
