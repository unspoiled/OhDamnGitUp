/*
 * Interface between LCD firmware and raspberry pi application.
 *
 * Authors: Levi Faid Zac Gardner Duncan Cowan Caleb Mitchell
 */

#include "lcd.h"

/**
 * Constructor. Connects to the i2c LCD and puts the LCD in 4 bit mode.
 *
 * @param addr the i2c address of the connected LCD.
 */
LCD::LCD(uint8_t lcd_addr) {
	_addr = lcd_addr;

	init();
}

/**
 * Initialize the LCD and put it into 4-bit mode.
 *
 * NOTE: The initialization and 4-bit mode specification came from the
 *       LCD's datasheet [https://www.sparkfun.com/datasheets/LCD/HD44780.pdf]
 *       page 46.
 */
void LCD::init() {
	// Initialize i2c
	i2cInit();

	_displayfunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8BITMAP;

	/* Wait for more than 40ms after VCC rises to 2.7 V */
	usleep(50000);

	// Keep RS and R/W pins low so we can send commands
	writeToScreen(LCD_BACKLIGHT);	// turn on the backlight
	usleep(1000);

	/* Put LCD into 4 bit mode as per page 46 of the datasheet */
	writeNibble(0x03 << 4);
	usleep(4500); // Wait for more than 4.1 ms

	writeNibble(0x03 << 4);
	usleep(4500); // Wait for more than 4.1 ms

	writeNibble(0x03 << 4);
	usleep(150); // Wait for more than 100 µs

	writeNibble(0x02 << 4);

	// Set the LCD to be in 4 bit mode, 2 lines high,
	// and use 5x8 bitmaps for the characters.
	command(LCD_FUNCTIONSET | _displayfunction);

	// Turn the LCD on with no blinking cursor.
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	command(LCD_DISPLAYCONTROL | _displaycontrol);

	// Set the text to go from right to left.
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	command(LCD_ENTRYMODESET | _displaymode);

	// Clear the screen.
	clear();
}

/**
 * Displays the specified string on the LCD.
 *
 * @param c the characters to print onto the LCD.
 */
void LCD::print(const char* c){
	uint8_t i;

	for(i = 0; i < strlen(c); i++)
		writeData(c[i]);
}

/**
 * Clears all characters from the screen and returns the cursor to the
 * start of the screen.
 */
void LCD::clear(){
	command(LCD_CLEARDISPLAY);
	usleep(2000); // Give it time to clear the LCD
}

/**
 * Sends the specified command to the LCD.
 */
void LCD::command(uint8_t value) {
	send(value, 0);
}

/**
 * Writes the specified byte to the LCD.
 */
void LCD::writeData(uint8_t value) {
	send(value, Rs);
}

/**
 * Initializes the underlining i2c connection between the raspberry pi and the LCD
 */
void LCD::i2cInit() {
	// Open the i2c device on the raspberry pi
	_i2cdevice = open(LCD_I2CDEVICE, O_RDWR);

	// Take control of the display
	ioctl(_i2cdevice, I2C_SLAVE, _addr);
}

/*
 * Sends commands and/or data to the LCD.
 *
 * @param value the value to set or send the LCD.
 * @param mode specifies what the value will be used for
 *        (i.e. mode=0 means the value will be a command).
 */
void LCD::send(uint8_t value, uint8_t mode) {
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
	writeNibble((highnib)|mode);
	writeNibble((lownib)|mode);
}

/**
 * Writes the specified nibble (4 bits) to the LCD.
 *
 * @param nibble the nibble to send to the LCD.
 */
void LCD::writeNibble(uint8_t nibble) {
	writeToScreen(nibble);
	pulseEnablePin(nibble);
}

/**
 * Writes the specified byte to the LCD.
 *
 * @param data the data to send to the LCD.
 */
void LCD::writeToScreen(uint8_t data){
	char *data_buffer;

	data_buffer = (char *)malloc(sizeof(char));
	data_buffer[0] = data | LCD_BACKLIGHT;

	// Send the data to the display
	write(_i2cdevice, data_buffer, 1);

	free(data_buffer);
}

/**
 * Pulses the enable pin high then low.
 *
 * This is like clicking the "Send" button on an email;
 * the data we just sent to the LCD was the body of the email,
 * and the pulsing of enable pin was clicking the send button.
 */
void LCD::pulseEnablePin(uint8_t _data){
	writeToScreen(_data | En);	// Enable pin high
	usleep(1);	// Enable pin must be on for >450ns

	writeToScreen(_data & ~En);	// Enable pin low
	usleep(50);	// Needs >37us for the  to settle
}
