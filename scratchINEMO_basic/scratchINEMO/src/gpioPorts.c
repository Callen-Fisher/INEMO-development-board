/*
 * gpioPorts.c
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */

#include "gpioPorts.h"

//there are a number of available GPIO pins. some of these have alternative functions
//GPIO_Pin_3	GPIOA	USART2_RX / TIM5_CH4 / ADC123_IN3 / TIM2_CH4
//GPIO_Pin_2	GPIOA	USART2_TX / TIM5_CH3 / ADC123_IN2 / TIM2_CH3
//GPIO_Pin_0	GPIOA	WKUP / USART2_CTS / ADC123_IN0 / TIM2_CH1_ETR / TIM5_CH1 / TIM8_ETR
//GPIO_Pin_11	GPIOA	USART1_CTS / USBDM / CAN_RX / TIM1_CH4
//GPIO_Pin_12	GPIOA	USART1_RTS / USBDP / CAN_TX / TIM1_ETR

//GPIO_Pin_6	GPIOB   I2C1_SCL / TIM4_CH1 with alternative function USART1_TX
//GPIO_Pin_7	GPIOB	I2C1_SDA / TIM4_CH2 with alternative function USART1_RX
//GPIO_Pin_9	GPIOB	TIM4_CH4 with alternative function I2C1_SDA / CAN_TX
//GPIO_Pin_8	GPIOB	TIM4_CH3 with alternative function I2C1_SCL / CAN_RX

//GPIO_Pin_10	GPIOA	USART1_RX / TIM1_CH3
//GPIO_Pin_9	GPIOA	USART1_TX / TIM1_CH2
//GPIO_Pin_4	GPIOA	SPI1_NSS / USART2_CK / DAC_OUT1 / ADC12_IN4
//GPIO_Pin_5	GPIOA	SPI1_SCK / DAC_OUT2 / ADC12_IN5
//GPIO_Pin_6	GPIOA	SPI1_MISO / TIM8_BKIN / ADC12_IN6 / TIM3_CH1 with alternative function TIM1_BKIN
//GPIO_Pin_7	GPIOA	SPI1_MOSI / TIM8_CH1N / ADC12_IN7 / TIM3_CH2 with alternative function TIM1_CH1N
//GPIO_Pin_1	GPIOA	USART2_RTS / ADC123_IN1 / TIM5_CH2 / TIM2_CH2

void setUpGPIO(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_12|GPIO_Pin_0|GPIO_Pin_10|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
