/*
 * ADC.h
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32f10x.h"
#include <stddef.h>
#include "DMA.h"

uint8_t readADCtempValue();
void setUpADC(void);
uint16_t AD_value;
uint16_t V25;// when V25=1.41V at ref 3.3V
uint16_t Avg_Slope; //when avg_slope=4.3mV/C at ref 3.3V
uint16_t TemperatureC;
void readADCdma(u16* out, float *motor_current_float);
void readADC_motorCurrent(float *output_float);

#endif /* ADC_H_ */
