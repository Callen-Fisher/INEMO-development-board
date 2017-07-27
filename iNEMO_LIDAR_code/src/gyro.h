/*
 * gyro.h
 *
 *  Created on: Sep 13, 2014
 *      Author: Callen Fisher
 */

#ifndef GYRO_H_
#define GYRO_H_

#include "stm32f10x.h"
#include <stddef.h>

void setUpGyro(void);
void initSPIgyro(void);
void spiGyroRegisterSetup(void);
uint8_t writeSPIgyro(uint8_t regAdr, uint8_t data);
void getGyro(uint8_t* data,float* out );
void getTempCelsius(uint8_t*temp);
s16 twosCompToDec16(u16 val);
s8 twosCompToDec8(uint8_t val);

uint8_t readADCtempValue();

#endif /* GYRO_H_ */
