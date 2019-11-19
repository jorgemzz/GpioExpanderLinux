/*
 * Max7315Driver.h
 *
 *  Created on: Nov 18, 2019
 *      Author: jorgemiranda
 */

#ifndef MAX7315DRIVER_MAX7315DRIVER_H_
#define MAX7315DRIVER_MAX7315DRIVER_H_

#include "../I2cDriver/I2cDriver.h"
#include <stdint.h>

#define MAX7315_IP_REGISTER 0x00
#define MAX7315_PH0OUT_REGISTER 0x01
#define MAX7315_PH1OUT_REGISTER 0x02
#define MAX7315_PORT_CONF_REGISTER 0x03

#define MAX7315_MASTER_INTENSITY_REGISTER 0x0E
#define MAX7315_CONFIGURATION_REGISTER 0x0F
#define MAX7315_INTENSITY_START_REGISTER 0x10

enum PinModes {INPUT, OUTPUT};
enum PwmModes {WITHOUT_PWM, CUSTOM_PWM, GLOBAL_PWM};

union Config_Reg{
	uint8_t all;
	struct{
		uint8_t BLINK_ENABLE:1;
		uint8_t BLINK_FLIP:1;
		uint8_t GLOBAL_INTENSITY:1;
		uint8_t INT_ENABLE:1;		//INTERRUPT ENABLE
		uint8_t INT_OUT_CONTROL:2;	//INTERRUPT OUTPUT CONTROL AS GPO
		uint8_t :1;
		uint8_t INT_STATUS:1;		//INTERRUPT STATUS
	};
};

union Nibble_Reg{
	uint8_t all;
	struct{
		uint8_t L_NIBBLE:4;
		uint8_t H_NIBBLE:4;
	};
};

class Max7315Driver : private I2cDriver{
private:
	uint8_t mDirection;
	uint8_t mPort;
	const int mI2cAddress;
	Config_Reg mConfiguration;
	Nibble_Reg mMasterIntensity;
	Nibble_Reg mOutputIntensity[4];

public:
	Max7315Driver(uint8_t i2cPort);
	~Max7315Driver();
	void PinMode(uint8_t pin, PinModes mode);
	void DigitalWrite(uint8_t pin, bool value);
	bool DigitalRead(uint8_t pin);
	void PwmMode(uint8_t pin ,PwmModes mode, Nibble_Reg value);
	void PwmWrite(uint8_t pin, uint8_t value);
};



#endif /* MAX7315DRIVER_MAX7315DRIVER_H_ */
