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
	myGpio.PinMode(6, OUTPUT);
	cout << "INPUT PIN1 level is: "  << myGpio.DigitalRead(1) << endl;
	cout << "INPUT PIN6 level is: "  << myGpio.DigitalRead(6) << endl;
	myGpio.DigitalWrite(1, true);
	myGpio.DigitalWrite(6, false);
	cout << "INPUT PIN1 level is: "  << myGpio.DigitalRead(1) << endl;
	cout << "INPUT PIN6 level is: "  << myGpio.DigitalRead(6) << endl;
	Nibble_Reg duty;
	duty.H_NIBBLE = 0x01;		//Master intensity timeslots, 01 timeslot
	duty.L_NIBBLE = 0x00;		//PWM cycles, 01 cycles
	myGpio.PwmMode(1, SINGLE_PWM, duty);
	duty.L_NIBBLE = 0x0e;		//PWM Cycles, 15 cycles
	myGpio.PwmMode(6, SINGLE_PWM, duty);
	return 0;
}
