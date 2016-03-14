/*
 * control.h
 *
 *  Created on: Sep 13, 2014
 *      Author: Callen Fisher
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include "stm32f10x.h"
#include <stddef.h>
#include "AccMag.h"
#include "gyro.h"

void controlMethod(float*acc,float*mag,float*gyro,s8*temp);

#endif /* CONTROL_H_ */
