// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// HDC1000
// This code is designed to work with the HDC1000_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Temperature?sku=HDC1000_I2CS#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if ((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, HDC1000 I2C address is 0x40(64)
	ioctl(file, I2C_SLAVE, 0x40);

	// Select configuration register(0x02)
	// Temperature, humidity enabled, resolultion = 14-bits, heater on(0x30)
	char config[2] = {0};
	config[0] = 0x02;
	config[1] = 0x30;
	write(file, config, 2);
	// Send temp measurement command(0x00)
	config[0] = 0x00;
	write(file, config, 2);
	sleep(1);

	// Read 2 bytes of data
	// temp msb, temp lsb
	char data[2] = {0};
	if(read(file, data, 2) != 2)
	{
		printf("Error : Input/Output error \n");
	}
	else
	{
		// Convert the data
		int temp = (data[0] * 256 + data[1]);
		float cTemp = (temp / 65536.0) * 165.0 - 40;
		float fTemp = cTemp * 1.8 + 32;

		// Output data to screen
		printf("Temperature in Celsius is : %.2f C \n", cTemp);
		printf("Temperature in Fahrenheit is : %.2f F \n", fTemp);
	}

	// Send humidity measurement command(0x01)
	config[0] = 0x01;
	write(file, config, 2);
	sleep(1);

	// Read 2 bytes of data
	// humidity msb, humidity lsb
	if(read(file, data, 2) != 2)
	{
		printf("Error : Input/Output error \n");
	}
	else
	{
		// Convert the data
		int hum = (data[0] * 256 + data[1]);
		float humidity = (hum / 65536.0) * 100.0;

		// Output data to screen
		printf("Relative Humidity : %.2f RH \n", humidity);
	}
}
