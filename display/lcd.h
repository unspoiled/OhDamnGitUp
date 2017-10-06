/*
 * lcd.h
 *
 * Authors: Levi Faid Zac Gardner Duncan Cowan Caleb Mitchell
 */

#ifndef LCD_H
#define LCD_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

extern "C" {
	#include <linux/i2c-dev.h>
	#include <sys/ioctl.h>
	#include <unistd.h>
	#include <fcntl.h>
}

#define LCD_I2CDEVICE "/dev/i2c-1"

// Commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_FUNCTIONSET 0x20

// Entry modes
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTDECREMENT 0x00

// Display control
#define LCD_DISPLAYON 0x04
#define LCD_CURSOROFF 0x00
#define LCD_BLINKOFF 0x00

// Function set
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_5x8BITMAP 0x00

// Backlight enable code
#define LCD_BACKLIGHT 0x08

#define En 0x04  // Enable bit
#define Rw 0x02  // Read/Write bit
#define Rs 0x01  // Register select bit

class LCD {
public:
	LCD(uint8_t addr);

	void print(const char*);
	void clear();

	void writeData(uint8_t);
	void command(uint8_t);

private:
	void init();
	void send(uint8_t, uint8_t);
	void writeNibble(uint8_t);
	void writeToScreen(uint8_t);
	void pulseEnablePin(uint8_t);
	void i2cInit();
	uint8_t _i2cdevice;
	uint8_t _addr;
	uint8_t _displayfunction;
	uint8_t _displaycontrol;
	uint8_t _displaymode;
};

#endif // LCD_H
