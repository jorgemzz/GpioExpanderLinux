/*
 * Max7315Driver.cpp
 *
 *  Created on: Nov 18, 2019
 *      Author: jorgemiranda
 */

#include "Max7315Driver.h"

Max7315Driver::Max7315Driver(uint8_t i2cPort) : I2cDriver(i2cPort),
												mDirection(0xFF),
												mPort(0x00),
												mI2cAddress(0x20){
	//Register default values:
	//Blink phase 0 outputs P7–P0:	0xFF
	//Ports configuration P7–P0:	0xFF
	//Blink phase 1 outputs P7–P0:	0xFF
	//Master, O8 intensity:			0x0F
	//Configuration:				0x0C
	mConfiguration.all = 0x0C;
	mMasterIntensity.all = 0x0F;
	for(int i = 0; i < 4; i++)
		mOutputIntensity[i].all = 0xFF;
}

Max7315Driver::~Max7315Driver(){

}

//======================================================================================
// Name        : PinMode
// Parameters  : uint8_t pin (the number of GPIO pin from 0 to 7)
//				 PinModes mode (one of the modes available for PIN configuration)
// Return	   : none
// Description : Configure a PIN as INPUT/OUTPUT.
//======================================================================================
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

//======================================================================================
// Name        : DigitalWrite
// Parameters  : uint8_t pin (the number of GPIO pin from 0 to 7)
//				 bool value (level to set the OUTPUT PIN to, 0 is LOW and 1 is HIGH)
// Return	   : none
// Description : Set the level of an OUTPUT PIN to LOW or HIGH.
//======================================================================================
void Max7315Driver::DigitalWrite(uint8_t pin, bool value){
	if(value)
		mPort |= ((uint8_t)1 << pin);
	else
		mPort &= ~((uint8_t)1 << pin);
	Buffer[1] = mPort;
	WriteData(mI2cAddress, MAX7315_PH0OUT_REGISTER, 2);
}

//======================================================================================
// Name        : DigitalRead
// Parameters  : uint8_t pin (the number of GPIO pin from 0 to 7)
// Return	   : bool (return the state of the INPUT PIN)
// Description : Read the current state of an INPUT PIN.
//======================================================================================
bool Max7315Driver::DigitalRead(uint8_t pin){
	ReadData(mI2cAddress, MAX7315_IP_REGISTER, 1);
	return (Buffer[0] & ((uint8_t)1 << pin))? true: false;
}

//======================================================================================
// Name        : PwmMode
// Parameters  : uint8_t pin (the number of GPIO pin from 0 to 7)
//				 PwmModes mode (one of the modes available for PWM configuration)
//				 Nibble_Reg value (variable with 02 nibbles, H_NIBBLE and L_NIBBLE,
//				 used to pass two values for PWM intensity configuration)
//				 Modes:
//				 WITHOUT_PWM--> Disable PWM function globally.
//								Parameters pin and value are Don’t care.
//				 SINGLE_PWM-->	Configure OUTPUT PIN as PWM output independently
//							 	value.H_NIBBLE = 4-bit global intensity
//								value.L_NIBBLE = 4-bit individual intensity.
//								Set L_NIBBLE to 0x0F to configure the pin as static output.
//				 GLOBAL_PWM-->	Configure all PINS to follow a common PWM configuration
//								value.H_NIBBLE = 4-bit master intensity timeslots
//								value.L_NIBBLE = 4-bit PWM cycles.
//				 There are 15 master/global intensity timeslots (value.H_NIBBLE = 1 to 15).
//				 Each master/global intensity timeslot contains 16 PWM cycles
//				 (value.L_NIBBLE = 0 to 15).
// Return	   : none
// Description : Configure the PWM registers related with a PIN.
//======================================================================================
void Max7315Driver::PwmMode(uint8_t pin ,PwmModes mode, Nibble_Reg value){
	switch(mode){
	case WITHOUT_PWM:
		//Set the master, O8 intensity register 0x0E to any value from 0x00 to 0x0F.
		mMasterIntensity.all = 0x0F;
		break;
	case SINGLE_PWM:
		//Set the master, O8 intensity register 0x0E to any value from 0x10 to 0xFF.
		//For the PWM outputs, set the output intensity value in the range 0x0 to 0xE.
		//Set Output Px intensity duty cycle to 16/16 (value.L_NIBBLE=0x0F) for static
		//logic level, no PWM.
		mMasterIntensity.H_NIBBLE = value.H_NIBBLE;	//High nibble is MASTER INTENSITY
		mConfiguration.GLOBAL_INTENSITY = 0;
		if(pin%2)
			mOutputIntensity[pin/2].H_NIBBLE = value.L_NIBBLE;
		else
			mOutputIntensity[pin/2].L_NIBBLE = value.L_NIBBLE;
		break;
	case GLOBAL_PWM:
	default:
		//Set the master, O8 intensity register 0x0E to any value from 0x10 to 0xFF.
		//Set O8 intensity duty cycle to 16/16 (value.L_NIBBLE=0x0F) for static output,
		//no PWM.
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
