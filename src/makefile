all: GpioExpander

GpioExpander: GpioExpander.o I2cDriver.o
	g++ -Wall -o GpioExpander GpioExpander.o I2cDriver/I2cDriver.o -lm

GpioExpander.o:
	g++ -Wall -c -o GpioExpander.o GpioExpander.cpp

I2cDriver.o: 
	g++ -Wall -c -o I2cDriver/I2cDriver.o I2cDriver/I2cDriver.cpp -DI2C_DEBUG

clean:
	rm -f *.o GpioExpander
	echo All done