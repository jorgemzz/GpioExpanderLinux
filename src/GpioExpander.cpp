//============================================================================
// Name        : GpioExpander.cpp
// Author      : jmiranda
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "Max7315Driver/Max7315Driver.h"

using namespace std;

int main() {
	cout << "GPIO Expander example" << endl;
	Max7315Driver myGpio(1);
	myGpio.PinMode(1, OUTPUT);
	Nibble_Reg duty;
	duty.H_NIBBLE = 0x00;
	duty.L_NIBBLE = 0x00;
	myGpio.PwmMode(1, CUSTOM_PWM, duty);
	//cout << "Readed data: "  << myGpio.DigitalRead(1) << endl;
	//cout << "Writting data ..." << endl;
	//myGpio.DigitalWrite(1, 0);
	//cout << "Readed data: "  << myGpio.DigitalRead(1) << endl;
	return 0;
}
