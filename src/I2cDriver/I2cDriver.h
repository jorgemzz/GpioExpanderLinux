/*
 * I2cDriver.h
 *
 *  Created on: Nov 15, 2019
 *      Author: jorgemiranda
 */

#ifndef I2CDRIVER_H_
#define I2CDRIVER_H_

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include <stdint.h>

class I2cDriver{
private:
	int mI2cFd;
	//int mLength;
	int mI2cBus;
	uint8_t mSlaveAddress;
	int mOpen();
	void mClose();
	int mSelectSlave(uint8_t slave);
public:
	I2cDriver(int bus);
	~I2cDriver();
	uint8_t Buffer[64];
	int ReadData(uint8_t slvAddr, uint8_t regAddress, uint8_t length);
	int WriteData(uint8_t slvAddr, uint8_t regAddress, uint8_t length);

};

#endif /* I2CDRIVER_H_ */
