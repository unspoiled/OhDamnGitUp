/*
 * Gives commands to the led drivers and processes data from the i2c bus.
 *
 * NOTE: To be used on a ATmega328 running the arduino bootloader.
 *
 * Author: Duncan Cowan
 */

#include <Wire.h>

#define I2C_ADDRESS 0x22

uint8_t *pixels;

const uint8_t CHARS[10][2][5] = {
    {{4}, {0x7E, 0x81, 0x81, 0x7E}}, // 0, width 4
    {{2}, {0x02, 0xFF}},             // 1, width 2
    {{4}, {0xC2, 0xA1, 0x91, 0x8E}}, // 2, width 4
    {{4}, {0x42, 0x89, 0x89, 0x76}}, // 3, width 4
    {{4}, {0x18, 0x14, 0x12, 0xFF}}, // 4, width 4
    {{4}, {0x8F, 0x89, 0x89, 0x71}}, // 5, width 4
    {{4}, {0x78, 0x94, 0x92, 0x61}}, // 6, width 4
    {{4}, {0xE1, 0x11, 0x09, 0x07}}, // 7, width 4
    {{4}, {0x76, 0x89, 0x89, 0x76}}, // 8, width 4
    {{4}, {0x86, 0x49, 0x29, 0x1E}}  // 9, width 4
  };

void setup() {
    // 24 coloumns, 8 rows
    pixels = (uint8_t*)calloc(24, sizeof(uint8_t));
  
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
    Serial.begin(9600);

    drawChar('1', 1, 0);
    drawChar('2', 6, 0);
    drawChar('3', 13, 0);
    drawChar('4', 18, 0);
    pixels[11] = 0x42;    // Colon
}

/*
 * Draws the character 'chr' to the display at the 'x' 'y' coordinates.
 */
void drawChar(uint8_t chr, uint8_t x, uint8_t y) {
    uint8_t width = CHARS[chr-48][0][0];
    const uint8_t *char_pixels = CHARS[chr-48][1];

    for(uint8_t i = 0; i < width; i++)
        pixels[(x+i)] = (uint8_t)((char_pixels[i] << y) & 0xFF);
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
            PORTD &= ~_BV(2);  // Clock low
            // TODO: describe these lines
            PORTD = (PORTD | _BV(4)) & (((first_driver_data >> i) << 4) | ~_BV(4));
            PORTD = (PORTD | _BV(5)) & (((second_driver_data >> i) << 5) | ~_BV(5));
            PORTD |= _BV(2);  // Clock high
        }
        PORTD |= _BV(6); // Disable leds before we transition to the next row, 
                         // stops random leds from flickering.
        PORTB = (PORTB | 0x3f) & ~_BV(j);
        PORTD |= _BV(3); // Latch high
        PORTD &= ~_BV(6); // Enable leds after we have fully transitioned.
        //delay(2);
    }
}

/*
 * Handles data recevied via the i2c bus.
 */
void receiveEvent(int numBytes) {
    while(Wire.available() > 0) {
        Serial.println(Wire.read(), BIN);
    }
}
