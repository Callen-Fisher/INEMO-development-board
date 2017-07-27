/*
 * control.h
 *
 *  Created on: Jun 9, 2014
 *      Author: Callen Fisher
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include "stm32f10x.h"
#include <stddef.h>
#include "AccMag.h"
#include "gyro.h"
#include "pwm.h"

void readEncoder(float *shaft_revs, float *shaft_speed);
void calibrateMEMS(float* acc, float* accCalib, float* gyro, float* gyroCalib, u8* temperature);
void controlMethod(float*acc,float*gyro,uint8_t*temp,float*angles,float*velocities,float*positions,
		float*PWMval,float*shaft_revs,uint8_t*activateControl);

#endif /* CONTROL_H_ */
