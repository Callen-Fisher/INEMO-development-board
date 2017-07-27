/*
 * DMA.h
 *
 *  Created on: Jun 8, 2014
 *      Author: Callen Fisher
 */

#ifndef DMA_H_
#define DMA_H_

#include "stm32f10x.h"
#include <stddef.h>

#define BufferSize 2
#define ADC1_DR    ((uint32_t)0x4001244C)

u16 ADC_ConvertedValue[BufferSize]; //BufferSize

void setUpDMA(u16* ADC_ConvertedValue);

#endif /* DMA_H_ */
