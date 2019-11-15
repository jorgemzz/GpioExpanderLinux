//============================================================================
// Name        : GpioExpander.cpp
// Author      : jmiranda
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "I2cDriver/I2cDriver.h"

using namespace std;

int main() {
	cout << "GPIO Expander example" << endl;
	I2cDriver myDevice(1);
	if(myDevice.ReadData(0x20, 0x00, 1))
		return -1;
	cout << "Readed data: " << myDevice.Buffer[0] << endl;
	return 0;
}
