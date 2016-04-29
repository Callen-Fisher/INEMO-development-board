/*
 * main.c
 *
 *  Created on: May 25, 2014
 *      Author: Callen Fisher
 */
//Include all the relevant (header) files
#include <stddef.h>
#include "stm32f10x.h"
#include "AccMag.h"
#include "controlLoopTimer.h"
#include "gpioPorts.h"
#include "gyro.h"
#include "testPorts.h"
#include "XBEE.h"
#include "DMA.h"
#include "ADC.h"
#include "control.h"
#include "send.h"

int main(void)
{
  //initialize all devices and ports here
  setUpLoopTimer();
  setUpGPIO();
  setUpEcompass();
  setUpGyro();
  setUpXbee();
  //initialize variables that are needed in the control loop
  float acc[3];
  float mag[3];
  float gyro[3];
  s8 temp;
  //start the control loop
  while (1)//main loop (the control loop, runs at the speed of the control loop timer)
  {
	  controlMethod(&acc[0],&mag[0],&gyro[0],&temp);
	  sendData(&acc[0],&mag[0],&gyro[0],&temp);
	  wait();//once everything is done, it hangs the processor until the end of the time limit
	  //50hz control loop, time limit for each loop is 20ms
  }
}
///////////////////////////////////////////////////////////////////////////
/////don't delete the following functions//////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}
