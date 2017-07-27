/*
 * controlLoopTimer.h
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */

#ifndef CONTROLLOOPTIMER_H_
#define CONTROLLOOPTIMER_H_

#include "stm32f10x.h"
#include <stddef.h>

uint8_t proceed;

void wait(void);
void resetTimInt(void);
#endif /* CONTROLLOOPTIMER_H_ */
