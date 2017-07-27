/*
 * lidar.h
 *
 *  Created on: Jun 12, 2017
 *      Author: Roberto Aldera
 */

#ifndef LIDAR_H_
#define LIDAR_H_

#include <stdbool.h>
#include <stm32f10x_i2c.h>
#include "I2C.h"
#include "controlLoopTimer.h"

#define LIDARLITE_ADDR_DEFAULT		0xC4 // 0x62 - 7bit address

//LIDARLite();
void setUpLIDAR(int configuration);
void configureLIDAR(int configuration);
void resetLIDAR(void);
int getDistanceLIDAR(bool biasCorrection);
void write(uint8_t myAddress, uint8_t myValue);
void read(char myAddress, int numOfBytes, char arrayToSave[2],
		bool monitorBusyFlag);
void correlationRecordToSerial(char separator, int numberOfReadings);

#endif /* LIDAR_H_ */
