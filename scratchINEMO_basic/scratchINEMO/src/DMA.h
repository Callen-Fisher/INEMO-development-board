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

#define BufferSize 5

void setUpDMA(u16* ADC_ConvertedValue);

#endif /* DMA_H_ */
