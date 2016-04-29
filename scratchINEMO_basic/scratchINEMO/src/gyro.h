/*
 * gyro.h
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */

#ifndef GYRO_H_
#define GYRO_H_

#include "stm32f10x.h"
#include <stddef.h>

void setUpGyro(void);
void initSPIgyro(void);
void spiGyroRegisterSetup(void);
uint16_t writeSPIgyro(uint8_t regAdr, uint8_t data);
void getGyro(float* data );
void getTemp(s8*temp);
s16 twosCompToDec16(u16 val);
s8 twosCompToDec8(u8 val);

#endif /* GYRO_H_ */
