/*
 * Gives commands to the led drivers and processes data from the i2c bus.
 *
 * NOTE: To be used on a ATmega328 running the arduino bootloader.
 *
 * Author: Duncan Cowan
 */

#include <Wire.h>

#define I2C_ADDRESS 0x22

// Codes for the different types of data formats
// the display understands.
#define DATA_PIXELS 0x0
#define DATA_PIXEL_AT_POSITION 0x10
#define DATA_STRING 0x20
#define DATA_CHAR 0x30

uint8_t *pixels;
uint8_t *pixel_buffer;

volatile bool swap_buffers = false;

// TODO: Put into program memory or EEPROM
// TODO: Create full ASCII character set
const uint8_t CHARS[12][2][5] = {
    {{4}, {0x7E, 0x81, 0x81, 0x7E}}, // 0, width 4
    {{2}, {0x02, 0xFF}},             // 1, width 2
    {{4}, {0xC2, 0xA1, 0x91, 0x8E}}, // 2, width 4
    {{4}, {0x42, 0x89, 0x89, 0x76}}, // 3, width 4
    {{4}, {0x18, 0x14, 0x12, 0xFF}}, // 4, width 4
    {{4}, {0x8F, 0x89, 0x89, 0x71}}, // 5, width 4
    {{4}, {0x78, 0x94, 0x92, 0x61}}, // 6, width 4
    {{4}, {0xE1, 0x11, 0x09, 0x07}}, // 7, width 4
    {{4}, {0x76, 0x89, 0x89, 0x76}}, // 8, width 4
    {{4}, {0x86, 0x49, 0x29, 0x1E}}, // 9, width 4
    {{1}, {0x42}},                   // COLON, width 1
    {{1}, {0x0}}                     // SPACE, width 1
  };

void setup() {
    // 24 coloumns, 8 rows
    pixels = (uint8_t*)calloc(24, sizeof(uint8_t));
    pixel_buffer = (uint8_t*)calloc(24, sizeof(uint8_t));
  
    // Turns all leds on
//    for(int i = 0; i < 24; i++)
//        pixels[i] = 0xFF;
  
    DDRD |= B01111100;  // Set pins 4, 5, 6, 11 and 12 as outputs.
    DDRB |= B00111111;  // Set pins 8 to 13 as outputs.
  
    PORTD &= B10000011; // Set pins 4, 5, 6, 11, and 12 low
    PORTB |= B00111111; // Set pins 7 to 13 high (which will turn the PNP transistors off)

    // I2c slave device setup
    Wire.begin(I2C_ADDRESS);
    Wire.onReceive(receiveEvent);

    // For debugging
//    Serial.begin(19200);
}

/*
 * Draws the character 'chr' to the display at the 'x' 'y' coordinates.
 */
void drawChar(uint8_t *p_buffer, uint8_t chr, uint8_t x, uint8_t y) {
    uint8_t width;
    const uint8_t *char_pixels;
    
    if(chr == ':') {                 // XXX: Tempory until full char set is implemented
        width = CHARS[10][0][0];
        char_pixels = CHARS[10][1];
    } else if(chr == ' ') {            // XXX: Tempory until full char set is implemented
        width = CHARS[11][0][0];
        char_pixels = CHARS[11][1];
    } else {
        width = CHARS[chr-48][0][0];
        char_pixels = CHARS[chr-48][1];
    }

    for(uint8_t i = 0; i < width; i++)
        p_buffer[(x+i)] = (uint8_t)((char_pixels[i] << y) & 0xFF);
}

/*
 * Updates the display as fast as possible.
 */
void loop() {
    uint16_t first_driver_data;
    uint16_t second_driver_data;
  
    for(uint8_t j = 0; j < 6; j++) {
        first_driver_data = ((pixels[j+6] << 8) | pixels[j]);
        second_driver_data = ((pixels[j+18] << 8) | pixels[j+12]);
    
        PORTD &= ~_BV(3); // Latch low
        for(uint8_t i = 0; i < 16; i++) {
            PORTD &= ~_BV(2);  // Clock low, get ready for incomming bit.
            // TODO: describe these lines
            PORTD = (PORTD | _BV(4)) & (((first_driver_data >> i) << 4) | ~_BV(4));
            PORTD = (PORTD | _BV(5)) & (((second_driver_data >> i) << 5) | ~_BV(5));
            PORTD |= _BV(2);  // Clock high, store the bit on the data in line.
        }
        PORTD |= _BV(6); // Disable leds before we transition to the next column, 
                         // stops random leds from flickering.
        PORTB = (PORTB | 0x3f) & ~_BV(j);   // Activate next column.
        PORTD |= _BV(3); // Latch high, put data in registers onto outputs.
        PORTD &= ~_BV(6); // Enable leds after we have fully transitioned.
        //delay(2);
    }

    if(swap_buffers)
        swapBuffers();
}

void swapBuffers() {
    uint8_t *temp;

    temp = pixels;
    pixels = pixel_buffer;
    pixel_buffer = temp;

    // Clear the buffer
    memset(pixel_buffer, 0, 24);

    swap_buffers = false;
}

/*
 * Handles data recevied via the i2c bus.
 */
void receiveEvent(int numBytes) {
    // Debugging
//    while(Wire.available() > 0) {
//        Serial.println(Wire.read(), BIN);
//    }
//    Serial.println();

    // Get the recived data's type (first nibble)
    uint8_t data_type;
    if(Wire.available() > 0)
        data_type = Wire.read() & 0xF0;
    else
        return;

    switch(data_type) {
        case DATA_PIXELS:
            // TODO: Implement setting the pixel array.
            break;
        case DATA_PIXEL_AT_POSITION:
            // TODO: Implement setting a single pixel.
            break;
        case DATA_STRING:
            int curr_char_offset;
            curr_char_offset = 0;
            
            while(Wire.available() > 0) {
                char chr =  Wire.read();

                // Draw each character in the string with a 1 pixel gap between them.
                drawChar(pixel_buffer, chr, curr_char_offset, 0);

                if(chr == ':') {                 // XXX: Tempory until full char set is implemented
                    curr_char_offset += CHARS[10][0][0] + 1;
                } else if(chr ==' ') {          // XXX: Tempory until full char set is implemented
                    curr_char_offset += CHARS[11][0][0] + 1;
                } else {
                    curr_char_offset += CHARS[chr-48][0][0] + 1;
                }
            }
            
            swap_buffers = true;
            break;
        case DATA_CHAR:
            // TODO: Implement single character drawing.
            break;
        default:
            // Unknown datatype. 
            // Should send error message to raspberry pi, 
            // but it's GPIO pins aren't 5v tollerent, 
            // so sending data to it might fry it.
            break;
    }

    // Flush the i2c receive buffer if there's any leftover data.
    while(Wire.available() > 0)
        Wire.read();
}
