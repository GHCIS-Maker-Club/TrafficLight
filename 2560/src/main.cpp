// colorwheel demo for Adafruit RGBmatrixPanel library.
// Renders a nice circle of hues on our 32x32 RGB LED matrix:
// http://www.adafruit.com/products/607
// 32x32 MATRICES DO NOT WORK WITH ARDUINO UNO or METRO 328.

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <RGBmatrixPanel.h>

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

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

// 定义一个缓冲区来存储显示内容
static const uint16_t RED_COLOR = matrix.Color333(7, 0, 0);

// 时间控制变量
const unsigned long REFRESH_INTERVAL = 3000; // 微秒，约333Hz刷新率
unsigned long lastRefreshTime = 0;

void setup() {
  matrix.begin();
  
  // 填充整个屏幕为红色
  for(int y = 0; y < matrix.width(); y++) {
    for(int x = 0; x < matrix.height(); x++) {
      matrix.drawPixel(x, y, RED_COLOR);
    }
  }
}

void loop() {
  unsigned long currentTime = micros();
  
  // 检查是否需要刷新显示
  if(currentTime - lastRefreshTime >= REFRESH_INTERVAL) {
    matrix.updateDisplay();
    lastRefreshTime = currentTime;
  }
}