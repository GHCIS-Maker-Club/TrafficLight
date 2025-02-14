#include <RGBmatrixPanel.h>

// 定义引脚
#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9   // 输出使能引脚
#define LAT 10   // 锁存引脚
#define A   A0   // 地址选择A
#define B   A1   // 地址选择B
#define C   A2   // 地址选择C

// 初始化16x16显示面板
// 最后一个参数true表示这是16x16的面板（而不是32x16）
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

void setup() {
  // 初始化矩阵
  matrix.begin();
  
  // 清除屏幕
  matrix.fillScreen(0);  // 先清空屏幕
  
  // 画出边框 - 使用绿色
  matrix.drawLine(0, 0, -15, 0, matrix.Color333(0, 7, 0));   // 上边
  matrix.drawLine(-15, 0, -15, -15, matrix.Color333(0, 7, 0)); // 右边
  matrix.drawLine(-15, -15, 0, -15, matrix.Color333(0, 7, 0)); // 下边
  matrix.drawLine(0, -15, 0, 0, matrix.Color333(0, 7, 0));   // 左边
}

void loop() {


}
