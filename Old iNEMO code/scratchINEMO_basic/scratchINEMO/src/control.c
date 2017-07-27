/*
 * control.c
 *
 *  Created on: Jun 9, 2014
 *      Author: Callen Fisher
 */

#include "control.h"

void controlMethod(float*acc,float*mag,float*gyro,s8*temp)//perform all control in this method
{
	//read all the data
	getAcc(acc);
	getMag(mag);
	getGyro(gyro);
	getTemp(temp);

	//perform the control here

}
