/*
 * display.h
 *
 * Author: Duncan Cowan
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

#define DISPLAY_I2C_ADDRESS 0x22

// Codes for the different types of data formats
// that can be sent to to the display.
#define DATA_PIXELS 0x0
#define DATA_PIXEL_AT_POSITION 0x10
#define DATA_STRING 0x20
#define DATA_CHAR 0x30

#define PIXEL_STATE_ON 0x1
#define PIXEL_STATE_OFF 0x0

#define SCROLL_NONE 0x0
#define SCROLL_LEFT 0x1
#define SCROLL_RIGHT 0x2
#define SCROLL_UP 0x3
#define SCROLL_DOWN 0x4

const int DISPLAY_WIDTH = 24;
const int DISPLAY_HEIGHT = 8;

extern int display_init();
extern int display_pixels(const char *pixels);
extern int display_pixel(char state, int x, int y);
extern int display_string(const char *str, int length, char scroll_dir, int x, int y);
extern int display_char(char chr, int x, int y);
extern const char* display_error();
extern void display_close();

#endif /* DISPLAY_H_ */
