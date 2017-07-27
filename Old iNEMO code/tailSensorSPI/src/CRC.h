/*
 * CRC.h
 *
 *  Created on: Sep 13, 2014
 *      Author: Callen Fisher
 */

#ifndef CRC_H_
#define CRC_H_

#include "stdint.h"

void initCRC();

uint32_t crcCalc(const uint8_t* data, uint16_t startIndex, uint16_t length);

#endif /* CRC_H_ */
