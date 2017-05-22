/*
 * Sends data to the led display via i2c.
 *
 * Author: Duncan Cowan
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

const int DISPLAY_I2C_ADDRESS = 0x22;

int main() {
	int i2c_device;
	char *filename = "/dev/i2c-1";

	// Tries to open the i2c device on the raspberry pi
	if((i2c_device = open(filename, O_RDWR)) < 0) {
		fprintf(stderr, "Could not open i2c device.\n");
		return 1;
	}

	// Tries to take control of the display
	if(ioctl(i2c_device, I2C_SLAVE, DISPLAY_I2C_ADDRESS) < 0) {
		fprintf(stderr, "Could not talk to device with address %d.\n", DISPLAY_I2C_ADDRESS);
		return 1;
	}

	// Creates some test data to send to the display
	int i, bytesToSend = 24;
	char buff[bytesToSend];
	for(i = 0; i < bytesToSend; i++)
		buff[i] = i%255;

	// Tries to send the data to the display
	if(write(i2c_device, buff, bytesToSend) < bytesToSend) {
		fprintf(stderr, "Could not write to display with address 0x%x.\n", DISPLAY_I2C_ADDRESS);
		return 1;
	}

	printf("Successfully sent %d bytes to the display with the address 0x%x.\n", bytesToSend, DISPLAY_I2C_ADDRESS);

	return 0;
}
