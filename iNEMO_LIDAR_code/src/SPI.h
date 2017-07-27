/*
 * SPI.h
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 */

#ifndef SPI_H_
#define SPI_H_

#include "stm32f10x.h"
#include <stddef.h>

//can only be SPI1
//you only need to change the CS stuff and maybe the phase and polarity
//if communicating to another device, ensure that you set up the registers for that device
//(see gyro.c for examples of setting up the registers)
#define CSPortClock 	RCC_APB2Periph_GPIOB		//the port of the chip select line
#define SPIMosi			GPIO_Pin_7
#define SPIMiso			GPIO_Pin_6
#define SPISck			GPIO_Pin_5
#define SPIPort			GPIOA
#define CSPin			GPIO_Pin_12					//the CS pin
#define CSPort			GPIOB						//the CS port
#define SPIPol			SPI_CPOL_Low				//can also be SPI_CPOL_High
#define SPIPhase		SPI_CPHA_2Edge				//can also be SPI_CPHA_1Edge

void initSPI(void);
int32_t writeSPI(uint8_t regAdr, uint8_t data);

#endif /* SPI_H_ */
