/*
 * lidar.c
 *
 *  Created on: Jun 12, 2017
 *      Author: Roberto Aldera
 */

#include "lidar.h"

/*------------------------------------------------------------------------------
 Constructor

 Use LIDARLite::begin to initialize.
 ------------------------------------------------------------------------------*/
//LIDARLite::LIDARLite() {}
/*------------------------------------------------------------------------------
 Begin

 Starts the sensor and I2C.

 Parameters
 ------------------------------------------------------------------------------
 configuration: Default 0. Selects one of several preset configurations.
 fasti2c: Default 100 kHz. I2C base frequency.
 If true I2C frequency is set to 400kHz.
 lidarliteAddress: Default 0x62. Fill in new address here if changed. See
 operating manual for instructions.
 ------------------------------------------------------------------------------*/
void setUpLIDAR(int configuration) {
//	Wire.begin(); // Start I2C
	for (int i = 0; i < 15; i++) {
		waitForEndTimerHz();
	}
	setUpI2C1();
//	if(fasti2c)
//	{
//#if ARDUINO >= 157
//		Wire.setClock(400000UL); // Set I2C frequency to 400kHz, for Arduino Due
//#else
//		TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
//#endif
//	}
	configureLIDAR(configuration); // Configuration settings
} /* LIDARLite::begin */

/*------------------------------------------------------------------------------
 Configure

 Selects one of several preset configurations.

 Parameters
 ------------------------------------------------------------------------------
 configuration:  Default 0.
 0: Default mode, balanced performance.
 1: Short range, high speed. Uses 0x1d maximum acquisition count.
 2: Default range, higher speed short range. Turns on quick termination
 detection for faster measurements at short range (with decreased
 accuracy)
 3: Maximum range. Uses 0xff maximum acquisition count.
 4: High sensitivity detection. Overrides default valid measurement detection
 algorithm, and uses a threshold value for high sensitivity and noise.
 5: Low sensitivity detection. Overrides default valid measurement detection
 algorithm, and uses a threshold value for low sensitivity and noise.
 lidarliteAddress: Default 0x62. Fill in new address here if changed. See
 operating manual for instructions.
 ------------------------------------------------------------------------------*/
void configureLIDAR(int configuration) {
	switch (configuration) {
	case 0: // Default mode, balanced performance
		write(0x02, 0x80); // Default
		write(0x04, 0x08); // Default
		write(0x1c, 0x00); // Default
		break;

	case 1: // Short range, high speed
		write(0x02, 0x1d);
		write(0x04, 0x08); // Default
		write(0x1c, 0x00); // Default
		break;

	case 2: // Default range, higher speed short range
		write(0x02, 0x80); // Default
		write(0x04, 0x00);
		write(0x1c, 0x00); // Default
		break;

	case 3: // Maximum range
		write(0x02, 0xff);
		write(0x04, 0x08); // Default
		write(0x1c, 0x00); // Default
		break;

	case 4: // High sensitivity detection, high erroneous measurements
		write(0x02, 0x80); // Default
		write(0x04, 0x08); // Default
		write(0x1c, 0x80);
		break;

	case 5: // Low sensitivity detection, low erroneous measurements
		write(0x02, 0x80); // Default
		write(0x04, 0x08); // Default
		write(0x1c, 0xb0);
		break;
	}
} /* LIDARLite::configure */

/*------------------------------------------------------------------------------
 Reset

 Reset device. The device reloads default register settings, including the
 default I2C address. Re-initialization takes approximately 22ms.

 Parameters
 ------------------------------------------------------------------------------
 lidarliteAddress: Default 0x62. Fill in new address here if changed. See
 operating manual for instructions.
 ------------------------------------------------------------------------------*/
void resetLIDAR() {
	write(0x00, 0x00);
} /* LIDARLite::reset */

/*------------------------------------------------------------------------------
 Distance

 Take a distance measurement and read the result.

 Process
 ------------------------------------------------------------------------------
 1.  Write 0x04 or 0x03 to register 0x00 to initiate an aquisition.
 2.  Read register 0x01 (this is handled in the read() command)
 - if the first bit is "1" then the sensor is busy, loop until the first
 bit is "0"
 - if the first bit is "0" then the sensor is ready
 3.  Read two bytes from register 0x8f and save
 4.  Shift the first value from 0x8f << 8 and add to second value from 0x8f.
 The result is the measured distance in centimeters.

 Parameters
 ------------------------------------------------------------------------------
 biasCorrection: Default true. Take aquisition with receiver bias
 correction. If set to false measurements will be faster. Receiver bias
 correction must be performed periodically. (e.g. 1 out of every 100
 readings).
 lidarliteAddress: Default 0x62. Fill in new address here if changed. See
 operating manual for instructions.
 ------------------------------------------------------------------------------*/
int getDistanceLIDAR(bool biasCorrection) {
	if (biasCorrection) {
		// Take acquisition & correlation processing with receiver bias correction
		write(0x00, 0x04);
	} else {
		// Take acquisition & correlation processing without receiver bias correction
		write(0x00, 0x03);
	}
	// Array to store high and low bytes of distance
	char distanceArray[2];
	// Read two bytes from register 0x8f (autoincrement for reading 0x0f and 0x10)
	read(0x8f, 2, distanceArray, true);
	// Shift high byte and add to low byte
	int distance = (distanceArray[0] << 8) + distanceArray[1];
	return (distance);
} /* LIDARLite::distance */

/*------------------------------------------------------------------------------
 Write

 Perform I2C write to device.

 Parameters
 ------------------------------------------------------------------------------
 myAddress: register address to write to.
 myValue: value to write.
 lidarliteAddress: Default 0x62. Fill in new address here if changed. See
 operating manual for instructions.
 ------------------------------------------------------------------------------*/
void write(uint8_t myAddress, uint8_t myValue) {
//	Wire.beginTransmission((int) lidarliteAddress);
	I2C_start(I2C1, LIDARLITE_ADDR_DEFAULT, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
//	Wire.write((int) myAddress); // Set register for write
	I2C_write(I2C1, myAddress); // write one byte to the slave
//	Wire.write((int) myValue); // Write myValue to register
	I2C_write(I2C1, myValue); // write one byte to the slave

	// A nack means the device is not responding, report the error over serial
//	int nackCatcher = Wire.endTransmission();
//	if (nackCatcher != 0) {
//		Serial.println("> nack");
//	}
	I2C_stop(I2C1); // stop the transmission
//	delay(1); // 1 ms delay for robustness with successive reads and writes
} /* LIDARLite::write */

/*------------------------------------------------------------------------------
 Read

 Perform I2C read from device. Will detect an unresponsive device and report
 the error over serial. The optional busy flag monitoring
 can be used to read registers that are updated at the end of a distance
 measurement to obtain the new data.

 Parameters
 ------------------------------------------------------------------------------
 myAddress: register address to read from.
 numOfBytes: numbers of bytes to read. Can be 1 or 2.
 arrayToSave: an array to store the read values.
 monitorBusyFlag: if true, the routine will repeatedly read the status
 register until the busy flag (LSB) is 0.
 ------------------------------------------------------------------------------*/
void read(char myAddress, int numOfBytes, char arrayToSave[2],
bool monitorBusyFlag) {
	int busyFlag = 0; // busyFlag monitors when the device is done with a measurement
	if (monitorBusyFlag) {
		busyFlag = 1; // Begin read immediately if not monitoring busy flag
	}
	int busyCounter = 0; // busyCounter counts number of times busy flag is checked, for timeout
	int sysStatus = 0;
	while (busyFlag != 0) // Loop until device is not busy
	{
		// Read status register to check busy flag
//		Wire.beginTransmission((int) lidarliteAddress);
		I2C_start(I2C1, LIDARLITE_ADDR_DEFAULT, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
//		Wire.write(0x01); // Set the status register to be read
		I2C_write(I2C1, 0x01); // write one byte to the slave

		// A nack means the device is not responding, report the error over serial
//		int nackCatcher = Wire.endTransmission();
//		if (nackCatcher != 0) {
//			Serial.println("> nack");
//		}
		I2C_stop(I2C1); // stop the transmission

		I2C_start(I2C1, LIDARLITE_ADDR_DEFAULT, I2C_Direction_Receiver); // start a transmission in Master receiver mode
//		Wire.requestFrom((int) lidarliteAddress, 1); // Read register 0x01
		sysStatus = I2C_read_nack(I2C1); // read one byte and don't request another byte
//		busyFlag = bitRead(Wire.read(), 0); // Assign the LSB of the status register to busyFlag
		busyFlag = sysStatus & 1;	//get LSB, the first bit

		busyCounter++; // Increment busyCounter for timeout

		// Handle timeout condition, exit while loop and goto bailout
//		if (busyCounter > 9999) {
//			goto bailout;
//		}
	}

	// Device is not busy, begin read
	if (busyFlag == 0) {
//		Wire.beginTransmission((int) lidarliteAddress);
		I2C_start(I2C1, LIDARLITE_ADDR_DEFAULT, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
//		Wire.write((int) myAddress); // Set the register to be read
		I2C_write(I2C1, myAddress); // write one byte to the slave

		// A nack means the device is not responding, report the error over serial
//		int nackCatcher = Wire.endTransmission();
//		if (nackCatcher != 0) {
//			Serial.println("> nack");
//		}
		I2C_stop(I2C1); // stop the transmission

		// Perform read of 1 or 2 bytes, save in arrayToSave
		I2C_start(I2C1, LIDARLITE_ADDR_DEFAULT, I2C_Direction_Receiver); // start a transmission in Master receiver mode
//		Wire.requestFrom((int) lidarliteAddress, numOfBytes);
		int i = 0;
//		if (numOfBytes <= Wire.available()) {
//		if (numOfBytes <= Wire.available()) {
		while (i < (numOfBytes - 1)) {
//				arrayToSave[i] = Wire.read();
			arrayToSave[i] = I2C_read_ack(I2C1); //maybe change this to nack in last read
			i++;
//			}
		}
		arrayToSave[i] = I2C_read_nack(I2C1);
	}

	// bailout reports error over serial
//	if (busyCounter > 9999) {
//		bailout: busyCounter = 0;
//		Serial.println("> read failed");
//}
} /* LIDARLite::read */

/*------------------------------------------------------------------------------
 Correlation Record To Serial

 The correlation record used to calculate distance can be read from the device.
 It has a bipolar wave shape, transitioning from a positive going portion to a
 roughly symmetrical negative going pulse. The point where the signal crosses
 zero represents the effective delay for the reference and return signals.

 Process
 ------------------------------------------------------------------------------
 1.  Take a distance reading (there is no correlation record without at least
 one distance reading being taken)
 2.  Select memory bank by writing 0xc0 to register 0x5d
 3.  Set test mode select by writing 0x07 to register 0x40
 4.  For as many readings as you want to take (max is 1024)
 1.  Read two bytes from 0xd2
 2.  The Low byte is the value from the record
 3.  The high byte is the sign from the record

 Parameters
 ------------------------------------------------------------------------------
 separator: the separator between serial data words
 numberOfReadings: Default: 256. Maximum of 1024
 lidarliteAddress: Default 0x62. Fill in new address here if changed. See
 operating manual for instructions.
 ------------------------------------------------------------------------------*/
void correlationRecordToSerial(char separator, int numberOfReadings) {

// Array to store read values
	char correlationArray[2];
// Var to store value of correlation record
	int correlationValue = 0;
//  Selects memory bank
	write(0x5d, 0xc0);
// Test mode enable
	write(0x40, 0x07);
	for (int i = 0; i < numberOfReadings; i++) {
		// Select single byte
		read(0xd2, 2, correlationArray, false);
		//  Low byte is the value of the correlation record
		correlationValue = correlationArray[0];
		// if upper byte lsb is set, the value is negative
		if ((int) correlationArray[1] == 1) {
			correlationValue |= 0xff00;
		}
//		Serial.print((int) correlationValue);
//		Serial.print(separator);
	}
// test mode disable
	write(0x40, 0x00);
} /* LIDARLite::correlationRecordToSerial */

void getLIDAR() {
//read LIDAR here
}
