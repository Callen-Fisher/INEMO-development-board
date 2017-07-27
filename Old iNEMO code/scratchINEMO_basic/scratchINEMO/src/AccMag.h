/*
 * AccMag.h
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */

#ifndef ACCMAG_H_
#define ACCMAG_H_

#include "stm32f10x.h"
#include <stddef.h>

void setUpEcompass(void);
void initI2CEcompass(void);
void initRegistersEcompass(void);

void eCompassRead(u8 address,uint32_t numByteToRead,u8* pBuffer);
void eCompassRead2(u8 address,uint32_t numByteToRead,u8* pBuffer);
void eCompassWrite(u8 data, u8 address);
void getAcc(float* out);
void getMag(float* out);
s16 twosCompToDec(u16 val);

#endif /* ACCMAG_H_ */
