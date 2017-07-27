/*
 * control.c
 *
 *  Created on: Jun 9, 2014
 *  Template: Callen Fisher
 *  Author: Roberto Aldera
 *
 */

#include "control.h"

//void calibrateMEMS(float* acc, float* accCalib, float* gyro, float* gyroCalib, u8* temperature){
//	accCalib[0] = 0.9922*acc[0] - 0.0247*acc[1] + 0.0225*acc[2] + 0.0468;
//	accCalib[1] = 0.0453*acc[0] + 0.9574*acc[1] + 0.0072*acc[2] + 0.022;
//	accCalib[2] = -0.0141*acc[0] -0.0037*acc[1] + 0.9971*acc[2] + 0.0053;
//
//	gyroCalib[0] = gyro[0] + (0.060394*temperature[1] - 3.0843);
//	gyroCalib[1] = gyro[1] + (0.025522*temperature[1] - 0.40084);
//	gyroCalib[2] = gyro[2] + (-0.32248*temperature[1] + 11.3496);
//}
