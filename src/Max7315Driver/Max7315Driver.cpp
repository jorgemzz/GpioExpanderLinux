/*
 * Max7315Driver.cpp
 *
 *  Created on: Nov 18, 2019
 *      Author: jorgemiranda
 */

#include "Max7315Driver.h"

Max7315Driver::Max7315Driver() : I2cDriver(1), mDirection(0xFF),mPort(0x00), mI2cAddress(0x20){

}

Max7315Driver::~Max7315Driver(){

}

void Max7315Driver::PinMode(uint8_t pin, PinModes mode){
	switch(mode){
	case OUTPUT:
		mDirection &= ~((uint8_t)1 << pin);
		break;
	case INPUT:
	default:
		mDirection |= ((uint8_t)1 << pin);
		break;
	}
	Buffer[1] = mDirection;
	WriteData(mI2cAddress, MAX7315_CONF_REGISTER, 2);
}

void Max7315Driver::DigitalWrite(uint8_t pin, bool value){
	if(value)
		mPort |= ((uint8_t)1 << pin);
	else
		mPort &= ~((uint8_t)1 << pin);
	Buffer[1] = mPort;
	WriteData(mI2cAddress, MAX7315_PH0OUT_REGISTER, 2);
}

bool Max7315Driver::DigitalRead(uint8_t pin){
	ReadData(mI2cAddress, MAX7315_IP_REGISTER, 1);
	return (Buffer[0] & ((uint8_t)1 << pin))? true: false;
}
