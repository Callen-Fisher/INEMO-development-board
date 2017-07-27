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
void getGyro(u8* data );
void getTemp(u8*temp);
s16 twosCompToDec16(u16 val);
s8 twosCompToDec8(u8 val);

uint8_t readADCtempValue();
void setUpADC(void);
uint16_t AD_value;
uint16_t V25;// when V25=1.41V at ref 3.3V
uint16_t Avg_Slope; //when avg_slope=4.3mV/C at ref 3.3V
uint16_t TemperatureC;
#endif /* GYRO_H_ */
