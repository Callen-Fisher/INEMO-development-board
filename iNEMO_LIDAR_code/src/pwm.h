/*
 * pwm.h
 *
 *  Created on: Oct 13, 2014
 *      Author: Callen Fisher
 */

#ifndef PWM_H_
#define PWM_H_

#include "stm32f10x.h"
void setUpPWM(void);
void setPWM(float* valPercentage);
#endif /* PWM_H_ */
