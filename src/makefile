all: GpioExpander

GpioExpander: GpioExpander.o Max7315Driver.o I2cDriver.o
	g++ -Wall -o GpioExpander GpioExpander.o I2cDriver/I2cDriver.o Max7315Driver/Max7315Driver.o -lm

GpioExpander.o:
	g++ -Wall -c -o GpioExpander.o GpioExpander.cpp

I2cDriver.o: 
	g++ -Wall -c -o I2cDriver/I2cDriver.o I2cDriver/I2cDriver.cpp -DI2C_DEBUG

Max7315Driver.o:
	g++ -Wall -c -o Max7315Driver/Max7315Driver.o Max7315Driver/Max7315Driver.cpp

clean:
	rm -f *.o GpioExpander I2cDriver/*.o Max7315Driver/*.o
	echo All done