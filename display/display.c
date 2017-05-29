/*
 * Interface between display firmware and raspberry pi application.
 *
 *  Author: Duncan Cowan
 */

#include "display.h"

const char *I2C_DEVICE_LOCATION = "/dev/i2c-1";

static int i2c_device;
static char error_msg[200];

static int _send_data(char data_type, const char *data, int length) {
	char *data_buffer;

	data_buffer = (char *)malloc((length+1)*sizeof(char));
	memcpy(data_buffer+1, data, length*sizeof(char));
	data_buffer[0] = data_type;

	// Tries to send the data to the display
	if(write(i2c_device, data_buffer, length+1) < length+1) {
		sprintf(error_msg, "Could not send data to display with address 0x%x.\n", DISPLAY_I2C_ADDRESS);
		free(data_buffer);
		return -1;
	}

	free(data_buffer);
	return 0;
}

int display_init() {
	// Tries to open the i2c device on the raspberry pi
	if((i2c_device = open(I2C_DEVICE_LOCATION, O_RDWR)) < 0) {
		sprintf(error_msg, "Could not open i2c device.\n");
		return -1;
	}

	// Tries to take control of the display
	if(ioctl(i2c_device, I2C_SLAVE, DISPLAY_I2C_ADDRESS) < 0) {
		sprintf(error_msg, "Could not talk to device with address %d.\n", DISPLAY_I2C_ADDRESS);
		return -1;
	}

	return 0;
}

int display_pixels(const char *pixels) {
	// header (1 byte): | data type [bits 4-7] | data type options (none) [bits 0-3] |
	// packet (24 bytes): | nth column of pixels [bits 0-7] |
	return _send_data(DATA_PIXELS, pixels, 24);
}

int display_pixel(char state, int x, int y) {
	char data_buffer[1];

	if(state > PIXEL_STATE_ON || state < PIXEL_STATE_OFF) {
		sprintf(error_msg, "Pixel state can only be PIXEL_STATE_ON (0x1) or PIXEL_STATE_OFF (0x0). state = %d\n", state);
		return -1;
	}

	if(x >= DISPLAY_WIDTH || x < 0) {
		sprintf(error_msg, "x must be within the range 0-%d. x = %d\n", DISPLAY_WIDTH-1, x);
		return -1;
	}

	if(y >= DISPLAY_HEIGHT || y < 0) {
		sprintf(error_msg, "y must be within the range 0-%d. y = %d\n", DISPLAY_HEIGHT-1, y);
		return -1;
	}

	// header (1 byte): | data type [bits 4-7] | data type options (pixel state) [bits 0-3] |
	// packet (1 byte): | pixel index (0-191) [bits 0-7] |
	data_buffer[0] = (char) (x + (DISPLAY_HEIGHT*y));
	return _send_data(DATA_PIXEL_AT_POSITION | state, data_buffer, 1);
}

int display_string(const char *str, int length, char scroll_dir, int x, int y) {

	if(length < 0) {
		sprintf(error_msg, "String length must be greater than 0. length = %d\n", length);
		return -1;
	}

	if(scroll_dir > SCROLL_DOWN || scroll_dir < 0) {
		sprintf(error_msg, "Undefined string scroll direction. scroll_dir = %d\n", scroll_dir);
		return -1;
	}

	if(x >= DISPLAY_WIDTH || x < 0) {
		sprintf(error_msg, "x must be within the range 0-%d. x = %d\n", DISPLAY_WIDTH-1, x);
		return -1;
	}

	if(y >= DISPLAY_HEIGHT || y < 0) {
		sprintf(error_msg, "y must be within the range 0-%d. y = %d\n", DISPLAY_HEIGHT-1, y);
		return -1;
	}

	// header (1 byte): | data type [bits 4-7] | data type options (scroll direction) [bits 0-3] |
	// packet (length bytes): | n char in str [bits 0-7] |
	return _send_data(DATA_STRING | scroll_dir, str, length);
}

int display_char(char chr, int x, int y) {
	char data_buffer[1];

	if(chr < 0 || chr > 255) {
		sprintf(error_msg, "chr must be within the range 0-255. chr = %d\n", chr);
		return -1;
	}

	if(x >= DISPLAY_WIDTH || x < 0) {
		sprintf(error_msg, "x must be within the range 0-%d. x = %d\n", DISPLAY_WIDTH-1, x);
		return -1;
	}

	if(y >= DISPLAY_HEIGHT || y < 0) {
		sprintf(error_msg, "y must be within the range 0-%d. y = %d\n", DISPLAY_HEIGHT-1, y);
		return -1;
	}

	// header (1 byte): | data type [bits 4-7] | data type options (none) [bits 0-3] |
	// packet (1 byte): | char [bits 0-7] |
	data_buffer[0] = chr;
	return _send_data(DATA_CHAR, data_buffer, 1);
}

const char* display_error() {
	return error_msg;
}

void display_close() {
	close(i2c_device);
}
