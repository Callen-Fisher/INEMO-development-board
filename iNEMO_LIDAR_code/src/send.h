/*
 * send.h
 *
 *  Created on: Jun 9, 2014
 *      Author: Callen Fisher
 */

#ifndef SEND_H_
#define SEND_H_

#include "stm32f10x.h"
#include <stddef.h>
#include "XBEE.h"

void sendData(float*acc,float*mag,float*gyro,s8*temp);

#endif /* SEND_H_ */
