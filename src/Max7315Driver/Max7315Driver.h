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

#define MAX7315_IP_REGISTER 0X00
#define MAX7315_PH0OUT_REGISTER 0X01
#define MAX7315_PH1OUT_REGISTER 0x02
#define MAX7315_CONF_REGISTER 0X03

enum PinModes {INPUT, OUTPUT};

class Max7315Driver : private I2cDriver{
private:
	uint8_t mDirection;
	uint8_t mPort;
	const int mI2cAddress;

public:
	Max7315Driver();
	~Max7315Driver();
	void PinMode(uint8_t pin, PinModes mode);
	void DigitalWrite(uint8_t pin, bool value);
	bool DigitalRead(uint8_t pin);

};



#endif /* MAX7315DRIVER_MAX7315DRIVER_H_ */
