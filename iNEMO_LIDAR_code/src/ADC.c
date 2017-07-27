/*
 * ADC.c
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */

//note this is code i found, it links the ADC to the DMA, this is set up for the ADC that
//is linked to the gyro, it needs to be changed for external ADC. This code has not been tested
//the DMA has been set up in the DMA.c file.

//Rob's source - http://www.embedds.com/multichannel-adc-using-dma-on-stm32/
#include "ADC.h"

void setUpADC() {
	//source: http://www.embedds.com/introducing-to-stm32-adc-programming-part2/
	V25 = 1750;    // when V25=1.41V at ref 3.3V
	Avg_Slope = 5; //when avg_slope=4.3mV/C at ref 3.3V

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//was disabled, but now we need multichannels
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//was disabled
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//was 1
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_TempSensorVrefintCmd(ENABLE);
	//temperature only reading in correctly when it's 1 and current sensor is 2:
	//	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_41Cycles5); // for current sensor, PA5
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_41Cycles5); // ADC_SampleTime_7Cycles5);
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);



	/* ADC2 configuration ------------------------------------------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC2, &ADC_InitStructure);
	/* ADC2 regular channels configuration */
	ADC_RegularChannelConfig(ADC2, ADC_Channel_5, 1, ADC_SampleTime_41Cycles5);
	/* Enable ADC2 EOC interrupt */
	ADC_ITConfig(ADC2, ADC_IT_EOC, ENABLE);


	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibration register */
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));

	/* Enable ADC2 */
	ADC_Cmd(ADC2, ENABLE);

	/* Enable ADC2 reset calibration register */
	ADC_ResetCalibration(ADC2);
	/* Check the end of ADC2 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC2));

	/* Start ADC2 calibration */
	ADC_StartCalibration(ADC2);
	/* Check the end of ADC2 calibration */
	while(ADC_GetCalibrationStatus(ADC2));

	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	/* Start ADC2 Software Conversion */
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);

	//ADC_Cmd(ADC1, ENABLE);
	//ADC_ResetCalibration(ADC1);
	//	while (ADC_GetResetCalibrationStatus(ADC1))
	//		;
	//
	//	ADC_StartCalibration(ADC1);
	//	while (ADC_GetCalibrationStatus(ADC1))
	//		;
	//set up the DMA
	//	setUpDMA((&ADC_ConvertedValue[0]));
	//	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
}
//uint8_t readADCtempValue() {
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
//	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)) {
//	}
//	AD_value = ADC_GetConversionValue(ADC1);
//	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
//	TemperatureC = (uint8_t) ((V25 - AD_value) / Avg_Slope + 25);
//	return TemperatureC;
//}


void readADCdma(u16* out, float *motor_current_float) {
	/* Enable ADC DMA */
	//	ADC_DMACmd(ADC1, ENABLE);
	//	ADC_Cmd(ADC1, ENABLE);
	//	DMA_Cmd(DMA1_Channel1, ENABLE);
	//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	//	while (!(DMA_GetFlagStatus(DMA1_FLAG_TC1)))
	//		;
	//	ADC_Cmd(ADC1, DISABLE);
	//	ADC_SoftwareStartConvCmd(ADC1, DISABLE);
	//	DMA_ClearFlag(DMA1_FLAG_TC1);
//	float tempMotorCurrent = 0;
//	tempMotorCurrent = -(ADC_GetConversionValue(ADC2)*(10.0/4096.0)*(3.30/5.0)-5.0)*1000.0;
	ADC_ConvertedValue[0] = ADC_GetConversionValue(ADC1);
//	ADC_ConvertedValue[1] = (uint16_t)(tempMotorCurrent);

	for (int i = 0; i < BufferSize; i++) {
		*out = ADC_ConvertedValue[i];
		out++;
	}
	*motor_current_float = -(ADC_GetConversionValue(ADC2)*(10.0/4096.0)*(3.30/5.0)-5.0);
}

void readADC_motorCurrent(float *output_float) {
	*output_float = -(ADC_GetConversionValue(ADC2)*(10.0/4096.0)*(3.30/5.0)-5.0);
}

void DMA1_Channel1_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_IT_TC1))
	{
		DMA_ClearITPendingBit(DMA1_IT_GL1);

		// Do something
	}
}
//void setUpADC(void) {
//	GPIO_InitTypeDef GPIO_InitStructure;
//	ADC_InitTypeDef ADC_InitStructure;
//
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0 | GPIO_Pin_2
//			| GPIO_Pin_4 | GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//
//	/* ADC configuration*/
//	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
//	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
//	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
//	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//	ADC_InitStructure.ADC_NbrOfChannel = 5;
//
//	/* ADC regular channel configuration */
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1,
//	ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2,
//	ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 3,
//	ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 4,
//	ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 5,
//	ADC_SampleTime_239Cycles5);
//
//	ADC_Init(ADC1, &ADC_InitStructure);
//
//	/* Enable ADC */
//	ADC_Cmd(ADC1, ENABLE);
//
//	/* Enable ADC reset calibration register */
//	ADC_ResetCalibration(ADC1);
//	/* Check the end of ADC reset calibration register */
//	while (ADC_GetResetCalibrationStatus(ADC1))
//		;
//
//	/* Start ADC calibration */
//	ADC_StartCalibration(ADC1);
//	/* Check the end of ADC calibration */
//	while (ADC_GetCalibrationStatus(ADC1))
//		;
//
//	//set up the DMA
//	setUpDMA((&ADC_ConvertedValue[0]));
//}

