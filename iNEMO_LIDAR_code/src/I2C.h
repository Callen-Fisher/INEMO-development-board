/*
 * I2C.h
 *
 *  Created on: Jun 6, 2014
 *      Author: Callen Fisher
 *      Updated by Roberto Aldera 2017
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f10x.h"
#include <stddef.h>
#include "AccMag.h"
//I2C1 setup
#define I2C_CLOCK_1			RCC_APB2Periph_GPIOB
#define	I2C_SCL_1			GPIO_Pin_8	//remapped from PB6
#define	I2C_SDA_1			GPIO_Pin_9	//remapped from PB7
#define I2C_PORT_1			GPIOB
#define I2C_SPEED_1			400000

//I2C2 setup
#define I2C_CLOCK_2			RCC_APB2Periph_GPIOB
#define	I2C_SCL_2			GPIO_Pin_10
#define	I2C_SDA_2			GPIO_Pin_11
#define I2C_PORT_2			GPIOB
#define I2C_SPEED_2			400000

void setUpI2C1(void);
void setUpI2C2(void);
void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
void I2C_stop(I2C_TypeDef* I2Cx);
void I2CRead(u8 address,uint32_t numByteToRead,u8* pBuffer);
void I2CWrite(u8 data, u8 address);	//for AccMag
void I2C_write(I2C_TypeDef* I2Cx, uint8_t data);	//for LIDAR
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx);
uint8_t I2C_read_nack(I2C_TypeDef* I2Cx);

#endif /* I2C_H_ */
