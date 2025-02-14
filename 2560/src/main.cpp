// testshapes demo for RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For 64x64 RGB LED matrix.

// WILL NOT FIT on ARDUINO UNO -- requires a Mega, M0 or M4 board

#include "RGBmatrixPanel.h"
#include "bit_bmp.h"
#include <string.h>
#include <stdlib.h>
// Most of the signal pins are configurable, but the CLK pin has some
// special constraints.  On 8-bit AVR boards it must be on PORTB...
// Pin 11 works on the Arduino Mega.  On 32-bit SAMD boards it must be
// on the same PORT as the RGB data pins (D2-D7)...
// Pin 8 works on the Adafruit Metro M0 or Arduino Zero,
// Pin A4 works on the Adafruit Metro M4 (if using the Adafruit RGB
// Matrix Shield, cut trace between CLK pads and run a wire to A4).

//#define CLK  8   // USE THIS ON ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
#define CLK 11 // USE THIS ON ARDUINO MEGA

#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3
#define E   A4

RGBmatrixPanel matrix(A, B, C, D, E, CLK, LAT, OE, false, 64);
//Configure the serial port to use the standard printf function

// Function declarations
void screen_clear();
void display_Image(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h);
void Demo_1();

void setup()
{
  Serial.begin(115200);
  matrix.begin();
  delay(500);
}

void loop()
{
  // Do nothing -- image doesn't change

 Demo_1();

}

//Clear screen
void screen_clear()
{
  matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 0));
}

/*  @name :  display_Image
 *  @brief:  display an image
 *           The image data is in the "bit_bmp.h"
 *           You can use some tools to get the image data
 *           You can set the data bits which is 8 or 16 and set the MSB first which is true or false on line 1001 of Adafruit_GFX.c
 *  @param:    x   Top left corner x coordinate
 *             y   Top left corner y coordinate
 *         bitmap  byte array with 16-bit color bitmap,the image data is in the "bit_bmp.h"
 *             w   Width of bitmap in pixels
 *             h   Height of bitmap in pixels
 *  @retval: None
 */
void display_Image(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h)
{
  matrix.display_image(x, y, bitmap, w, h);
}

void Demo_1()
{
  screen_clear();
  display_Image(0, 0, gImage_image, 64, 64);
  delay(2000);
}

