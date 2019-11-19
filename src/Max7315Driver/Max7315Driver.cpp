/*
 * Max7315Driver.cpp
 *
 *  Created on: Nov 18, 2019
 *      Author: jorgemiranda
 */

#include "Max7315Driver.h"

Max7315Driver::Max7315Driver(uint8_t i2cPort) : I2cDriver(i2cPort), mDirection(0xFF),mPort(0x00), mI2cAddress(0x20){
	//Register default values
	//Blink phase 0 outputs P7–P0:	0xff
	//Ports configuration P7–P0:	0xff
	//Blink phase 1 outputs P7–P0:	0xff
	//Master, O8 intensity:			0x0f
	//Configuration:				0x0c
	mConfiguration.all = 0x0C;
	mMasterIntensity.all = 0x0F;
	for(int i = 0; i < 4; i++)
		mOutputIntensity[i].all = 0xFF;
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
	WriteData(mI2cAddress, MAX7315_PORT_CONF_REGISTER, 2);
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

void Max7315Driver::PwmMode(uint8_t pin ,PwmModes mode, Nibble_Reg value){
	switch(mode){
	case WITHOUT_PWM:
		//Set the master, O8 intensity register 0x0E to any value from 0x00 to 0x0F.
		mMasterIntensity.all = 0x0F;
		break;
	case CUSTOM_PWM:
		//For the static outputs, set the output intensity value to 0xF.
		//For the PWM outputs, set the output intensity value in the range 0x0 to 0xE.
		//Set the master, O8 intensity register 0x0E to any value from 0x10 to 0xFF.
		mMasterIntensity.H_NIBBLE = value.H_NIBBLE;	//Only High nibble produce change
		mConfiguration.GLOBAL_INTENSITY = 0;
		if(pin%2)
			mOutputIntensity[pin/2].H_NIBBLE = value.L_NIBBLE;
		else
			mOutputIntensity[pin/2].L_NIBBLE = value.L_NIBBLE;
		break;
	case GLOBAL_PWM:
	default:
		//Set the master, O8 intensity register 0x0E to any value from 0x10 to 0xFF.
		mMasterIntensity.all = value.all;
		mConfiguration.GLOBAL_INTENSITY = 1;
		break;
	}

	Buffer[1] = mMasterIntensity.all;
	WriteData(mI2cAddress, MAX7315_MASTER_INTENSITY_REGISTER, 2);
	Buffer[1] = mConfiguration.all;
	WriteData(mI2cAddress, MAX7315_CONFIGURATION_REGISTER, 2);
	for(int i = 0; i < 4; i++){
		Buffer[i+1] = mOutputIntensity[i].all;
	}
	WriteData(mI2cAddress, MAX7315_INTENSITY_START_REGISTER, 5);
}
