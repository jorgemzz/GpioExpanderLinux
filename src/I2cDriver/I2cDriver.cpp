/*
 * I2cDriver.cpp
 *
 *  Created on: Nov 15, 2019
 *      Author: jorgemiranda
 */

#include "I2cDriver.h"
#include <string.h>
#include <iostream>

I2cDriver::I2cDriver(int bus){
	mI2cFd =  0;
	mI2cBus = bus;
}

I2cDriver::~I2cDriver(){

}

int I2cDriver::mOpen(){
	std::string portName = "/dev/i2c-" + std::to_string(mI2cBus);
	if(!mI2cFd){
#ifdef I2C_DEBUG
		std::cout << "I2cDriver::mOpen: " << portName << std::endl;
#endif
		if((mI2cFd = open(portName.c_str(), O_RDWR)) < 0){
#ifdef I2C_DEBUG
		std::cout << "I2cDriver::mOpen: " << "Failed to open the i2c bus" << std::endl;
#endif
			mI2cFd = 0;
			return -1;
		}
	}
	return 0;
}

void I2cDriver::mClose(){
	if(!mI2cFd)
		return;
	close(mI2cFd);
	mI2cFd = 0;
}

int I2cDriver::mSelectSlave(uint8_t slaveAddress){
	if(mSlaveAddress == slaveAddress)
		return 0;
	if(mOpen())
		return -1;
#ifdef I2C_DEBUG
	std::cout << "I2cDriver::SelectSlave: 0x" << std::hex << (int)slaveAddress << std::endl;
#endif
	if((ioctl(mI2cFd, I2C_SLAVE, slaveAddress)) < 0){
#ifdef I2C_DEBUG
		std::cout << "I2cDriver::SelectSlave: " << "Failed to acquire bus access and/or talk to slave" << std::endl;
#endif
		return -1;
	}
	mSlaveAddress = slaveAddress;
	return 0;
}

int I2cDriver::ReadData(uint8_t slvAddr, uint8_t regAddress, uint8_t length){
#ifdef I2C_DEBUG
	std::cout << "I2cDriver::ReadData: address 0x" << std::hex << (int)regAddress << ", length " << (int)length << std::endl;
#endif
	if(WriteData(slvAddr, regAddress, 1))
		return -1;
	if(read(mI2cFd, Buffer, length) != length){
#ifdef I2C_DEBUG
		std::cout << "I2cDriver::ReadData: " << "Failed to read from the i2c bus" << std::endl;
#endif
		return -1;
	}
	return 0;
}

int I2cDriver::WriteData(uint8_t slvAddr, uint8_t regAddress, uint8_t length){
#ifdef I2C_DEBUG
	std::cout << "I2cDriver::WriteData: address 0x" << std::hex << (int)regAddress << ", length " << (int)length << std::endl;
	for(int j=1; j < length; j++)
		std::cout << "\tData[" << j << "]=" << std::hex <<  (int)Buffer[j] << std::endl;
#endif
	if(mSelectSlave(slvAddr))
		return -1;
	Buffer[0] = regAddress;
	if(write(mI2cFd, Buffer, length) != length){
#ifdef I2C_DEBUG
		std::cout << "I2cDriver::WriteData: " << "Failed to write from the i2c bus" << std::endl;
#endif
		return -1;
	}
	return 0;
}
