/*
 * main.c
 *
 *  Created on: May 25, 2014
 *  Original author: Callen Fisher
 *  Current author: Roberto Aldera, cheetah project 2017
 */
//Include all the relevant (header) files
//#include <stddef.h>
#include "stm32f10x.h"
#include "AccMag.h"
#include "controlLoopTimer.h"
#include "gpioPorts.h"
#include "gyro.h"
#include "testPorts.h"
#include "XBEE.h"
#include "USART.h"
#include "DMA.h"
#include "ADC.h"
#include "control.h"
#include "send.h"
#include "serial_terminal.h"
#include "CRC.h"
#include "pwm.h"
#include "lidar.h"

uint8_t acc8[6];		//3 axes, each returning uint16_t, 3x2=6
uint8_t mag8[6];
uint8_t gyro8[6];
uint8_t angles8[12];
uint8_t MiscPayload8[17];//temperature=1 (just ADCtemp),PWM=4,Encoder=4+4, currentSensor=4
uint16_t encoderPosition = 0;
uint16_t adcValDMA[2];	//to store temperature and current sensor readings
uint16_t testVar[2];	//debugging purposes
uint8_t temperature[2];
uint8_t status = 0;
uint8_t XBee_waiting = 0;
uint8_t activateControl = 0;
float acc[3];
float accCalib[3];
float mag[3];
float gyro[3];
float gyroCalib[3];
float angles[3];
float velocities[3];
float positions[3];
float PWMval;
float shaft_revs = 0;
float shaft_speed = 0;
float motorCurrent;	//ADC reading from motor current sensor

void setUpEncoder();
void convertDataToBytes(void);
void toggleGPIOpin(uint8_t *status);

int main(void) {
	int i = 0;
//	int tempDistance = 0;
	serialTerminal_Init();
	setUpLoopTimer();
//	setUpEncoder();
	setUpGPIO();	//TODO check this later.
	setUpEcompass();	//I2C halts main loop if we don't set this up...
	setUpGyro();
	setUpADC();	//this also sets up DMA...
	setUpLoggerSDcard();
	setUpPWM();
//	setUpLIDAR(0);		//0 ---> default settings

	while (1) {
		toggleGPIOpin(&status);

		//not yet included in packet
		//TRUE: Take a measurement with receiver bias correction
		//FALSE: Take a measurement without receiver bias correction
		//LIDAR needs 4 wires - red/black for 5V/GND, and green/blue for SCL/SDA on PB8/PB9
//		tempDistance = getDistanceLIDAR(false);

		getAcc(acc8, acc);
		getGyro(gyro8, gyro);
		readADCdma(adcValDMA, &motorCurrent);//for getting temperature, a u16 val
		temperature[1] = (uint8_t) ((V25 - adcValDMA[0]) / Avg_Slope + 25);

		for (int p = 0; p < sizeof(TxBuff); p++) {
			TxBuff[p] = 0;
		}

		DMA_Cmd(DMA1_Channel7, DISABLE);
		DMA_SetCurrDataCounter(DMA1_Channel7, sizeof(TxBuff));
		DMA_Cmd(DMA1_Channel4, DISABLE);
		DMA_SetCurrDataCounter(DMA1_Channel4, sizeof(TxBuff));

		//Four payloads, gyro/acc/angles/MiscPayload
		convertDataToBytes();
		serialTerminal_packetize(gyro8, acc8, angles8, MiscPayload8,
				sizeof(gyro8), sizeof(acc8), sizeof(angles8),
				sizeof(MiscPayload8));

		//SD card data
		DMA_Cmd(DMA1_Channel4, ENABLE);
		USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
		USART_Cmd(USART1, ENABLE);

		//Toggle PA11 to test loop frequency
//		toggleGPIOpin(&status);
		//wait until next run, to maintain 200Hz sync
		waitForEndTimerHz();
		i++;
	}
}

void toggleGPIOpin(uint8_t *status) {
	if (*status == 0) {
		GPIO_SetBits(GPIOA, GPIO_Pin_11);
		*status = 1;
	} else {
		GPIO_ResetBits(GPIOA, GPIO_Pin_11);
		*status = 0;
	}
}

void convertDataToBytes() {
	union {
		char temp_char[12];
		float temp_int_buffer[3];
	} buffer_to_char_union1;
	buffer_to_char_union1.temp_int_buffer[0] = angles[0];
	buffer_to_char_union1.temp_int_buffer[1] = angles[1];
	buffer_to_char_union1.temp_int_buffer[2] = angles[2];
	for (int i = 0; i < 12; i++) {
		angles8[i] = buffer_to_char_union1.temp_char[i];
	}

	//	union {
	//		uint8_t temp_signed[2];
	//		uint8_t temp_unsigned[2];
	//	} buffer_to_char_union2;
	//	//temperatureToPack[1] = 30;
	//	buffer_to_char_union2.temp_signed[0] = temperature[0];
	//	buffer_to_char_union2.temp_signed[1] = temperature[1];
	//	for (int i = 0; i < 2; i++) {
	//		MiscPayload8[i] = buffer_to_char_union2.temp_unsigned[i];
	//	}
	for (int i = 0; i < 1; i++) {
		//		MiscPayload8[i] = temperature[i];
		MiscPayload8[0] = temperature[1];
	}

	union {
		char temp_char[4];
		float temp_int_buffer;
	} buffer_to_char_union3;
	buffer_to_char_union3.temp_int_buffer = PWMval;
	for (int i = 1; i < 5; i++) {
		MiscPayload8[i] = buffer_to_char_union3.temp_char[i - 1];
	}

	union {
		char temp_char[4];
		float temp_int_buffer;
	} buffer_to_char_union4;
	buffer_to_char_union4.temp_int_buffer = shaft_revs;
	for (int i = 5; i < 9; i++) {
		MiscPayload8[i] = buffer_to_char_union4.temp_char[i - 5];
	}

	union {
		char temp_char[4];
		float temp_int_buffer;
	} buffer_to_char_union5;
	buffer_to_char_union5.temp_int_buffer = shaft_speed;
	for (int i = 9; i < 13; i++) {
		MiscPayload8[i] = buffer_to_char_union5.temp_char[i - 9];
	}

	union {
		char temp_char[4];
		float temp_int_buffer;
	} buffer_to_char_union6;
	buffer_to_char_union6.temp_int_buffer = motorCurrent;
	for (int i = 13; i < 17; i++) {
		MiscPayload8[i] = buffer_to_char_union6.temp_char[i - 13];
	}
}

///////////////////////////////////////////////////////////////////////////
/////don't delete the following functions//////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void __assert_func(const char *file, int line, const char *func,
		const char *failedexpr) {
	while (1) {
	}
}
void __assert(const char *file, int line, const char *failedexpr) {
	__assert_func(file, line, NULL, failedexpr);
}
