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

void setUpADC(void);
void readADCdma(u16* out);

#endif /* ADC_H_ */
