/*
 * I2C.h
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f10x.h"
#include <stddef.h>
#define I2Cclock		RCC_APB1Periph_I2C1
#define I2CportClock	RCC_APB2Periph_GPIOB
#define I2Cscl			GPIO_Pin_6				//can also be GPIO_Pin_8
#define I2Csda			GPIO_Pin_7				//can also be GPIO_Pin_9
#define I2Cport			GPIOB

void initI2C(void);
void I2CRead(u8 address,uint32_t numByteToRead,u8* pBuffer);
void I2CWrite(u8 data, u8 address);

#endif /* I2C_H_ */
