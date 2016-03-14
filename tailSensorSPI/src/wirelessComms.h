/*
 * wirelessComms.h
 *
 *  Created on: Nov 25, 2014
 *      Author: Callen Fisher
 */

#ifndef WIRELESSCOMMS_H_
#define WIRELESSCOMMS_H_

#include "stm32f10x.h"
#include <stddef.h>
#include "serial_terminal.h"
#include "main.h"

#define SENSORADDRESS 0xB8

void initSPI(void);
void setUpRegisters(void);
void chipSelect(void);
void chipDeSelect(void);
void setUpRegisters(void);
void delay100ms(void);
void delay130micro(void);
void chipEnable(void);
void chipDisable(void);
uint8_t writeSPIcomms(uint8_t regAdr, uint8_t data);
void readPacket(uint8_t* data,uint8_t length);
void setADDR(uint8_t Pipe, uint8_t one, uint8_t two, uint8_t three, uint8_t four, uint8_t five);
void setADDRshort(uint8_t Pipe, uint8_t one);
void flushRX(void);
void flushTX(void);
void addAckPacket(uint8_t* packetByte,uint8_t size);

uint8_t dataRXPacket[1];
uint8_t dataTXtemp[2];
uint8_t dataTXdata[18];

#endif /* WIRELESSCOMMS_H_ */
