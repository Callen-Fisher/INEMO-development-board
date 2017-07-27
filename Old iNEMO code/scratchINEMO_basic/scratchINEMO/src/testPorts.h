/*
 * testPorts.h
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */

#ifndef TESTPORTS_H_
#define TESTPORTS_H_

#include "stm32f10x.h"
#include <stddef.h>

void test(void);
void initGPIO(void);
void flash(void);
void del(void);

#endif /* TESTPORTS_H_ */
