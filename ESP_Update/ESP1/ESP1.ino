// Traffic Light Control System for LED Matrix Display
// LED矩阵显示屏交通灯控制系统

//---PIN DEFINITION 引脚定义---
// LED Matrix Panel Pins LED矩阵面板引脚
#define R1 25      // Red channel 1 红色通道1
#define G1 26      // Green channel 1 绿色通道1
#define BL1 27     // Blue channel 1 蓝色通道1
#define R2 14      // Red channel 2 红色通道2
#define G2 12      // Green channel 2 绿色通道2
#define BL2 13     // Blue channel 2 蓝色通道2
#define CH_A 23    // Address channel A 地址通道A
#define CH_B 19    // Address channel B 地址通道B
#define CH_C 5     // Address channel C 地址通道C
#define CH_D 17    // Address channel D 地址通道D
#define CH_E 18    // Address channel E (for 64x64 panels) 地址通道E（用于64x64面板）
#define CLK 16     // Clock signal 时钟信号
#define LAT 4      // Latch signal 锁存信号
#define OE 15      // Output enable 输出使能

// State Control Pins 状态控制引脚
#define STATES_PIN_1 2   // State control pin 1 状态控制引脚1
#define STATES_PIN_2 21  // State control pin 2 状态控制引脚2
#define STATES_PIN_3 22  // State control pin 3 状态控制引脚3
#define STATES_PIN_4 33  // State control pin 4 状态控制引脚4

// Display States 显示状态定义
#define DISPLAY_RED 0      // Red light state 红灯状态
#define DISPLAY_YELLOW 1   // Yellow light state 黄灯状态
#define DISPLAY_GREEN 2    // Green light state 绿灯状态
#define CLEAR_ALL 3        // Clear display state 清除显示状态
#define ANIMATION_1 4      // Animation state 1 动画状态1
#define ANIMATION_2 5      // Animation state 2 动画状态2
#define ANIMATION_3 6      // Animation state 3 动画状态3
#define ANIMATION_4 7      // Animation state 4 动画状态4
#define ANIMATION_5 8      // Animation state 5 动画状态5
#define ANIMATION_6 9      // Animation state 6 动画状态6
#define ANIMATION_7 10     // Animation state 7 动画状态7
#define ANIMATINO_8 11     // Animation state 8 动画状态8

#define THIS_COLOR 0       // Current traffic light color (0=RED) 当前交通灯颜色（0=红色）

// Library Includes 库文件包含
#include "ESP32-HUB75-MatrixPanel-I2S-DMA.h"  // LED Matrix Driver Library LED矩阵驱动库
#include <FastLED.h>                          // LED Control Library LED控制库
#include "Image.h"                            // Custom Image Data 自定义图像数据

// Panel Configuration 面板配置
#define PANEL_WIDTH 64     // Single panel width 单个面板宽度
#define PANEL_HEIGHT 64    // Single panel height 单个面板高度
#define PANELS_NUMBER 1    // Number of chained panels 级联面板数量
#define PIN_E 18          // E pin for panel control E引脚用于面板控制

// Display Dimensions 显示尺寸
#define PANE_WIDTH PANEL_WIDTH * PANELS_NUMBER  // Total display width 总显示宽度
#define PANE_HEIGHT PANEL_HEIGHT                // Total display height 总显示高度

// Image Configuration 图像配置
const int IMAGE_WIDTH = 64;   // Image width 图像宽度
const int IMAGE_HEIGHT = 64;  // Image height 图像高度

// Display Control Variables 显示控制变量
MatrixPanel_I2S_DMA *dma_display = nullptr;    // Matrix panel object 矩阵面板对象
uint16_t time_counter = 0, cycles = 0, fps = 0;  // Performance counters 性能计数器
unsigned long fps_timer;                        // FPS timer FPS计时器
uint8_t brightness = 255;                      // Display brightness 显示亮度

void setup() {
    // Initialize state control pins 初始化状态控制引脚
    pinMode(STATES_PIN_1, INPUT_PULLDOWN);
    pinMode(STATES_PIN_2, INPUT_PULLDOWN);
    pinMode(STATES_PIN_3, INPUT_PULLDOWN);
    pinMode(STATES_PIN_4, INPUT_PULLDOWN);

    // Initialize serial communication 初始化串口通信
    Serial.begin(9600);
    
    Serial.println(F("*****************************************************"));
    Serial.println(F("*        ESP32 PICTURE DISPLAY        *"));
    Serial.println(F("*****************************************************"));
    
    // Matrix panel configuration 矩阵面板配置
    HUB75_I2S_CFG mxconfig;
    mxconfig.mx_height = PANEL_HEIGHT;      // Set panel height 设置面板高度
    mxconfig.chain_length = PANELS_NUMBER;  // Set number of panels 设置面板数量
    mxconfig.gpio.e = PIN_E;               // Configure E pin 配置E引脚
    mxconfig.driver = HUB75_I2S_CFG::FM6124;  // Set panel driver 设置面板驱动类型

    // Initialize display 初始化显示
    dma_display = new MatrixPanel_I2S_DMA(mxconfig);
    dma_display->setBrightness8(brightness);   
    if( not dma_display->begin() )
        Serial.println("****** !KABOOM! I2S memory allocation failed ***********");
    
    Serial.println("Fill screen: RED");
    delay(100);
    Serial.println("Starting image display...");
    fps_timer = millis();

    // Set initial brightness 设置初始亮度
    dma_display -> setBrightness8(128);
}

// Animation control variables 动画控制变量
int cur_gif;               // Current animation index 当前动画索引
const int max_gif = 10;    // Maximum number of animations 最大动画数量

// State detection function 状态检测函数
int new_state(){
    // Read state control pins 读取状态控制引脚
    int bit_1 = digitalRead(STATES_PIN_1);
    int bit_2 = digitalRead(STATES_PIN_2);
    int bit_3 = digitalRead(STATES_PIN_3);
    int bit_4 = digitalRead(STATES_PIN_4);
    // Combine bits to form state number 组合位形成状态数
    return (bit_1 << 3) + (bit_2 << 2) + (bit_3 << 1) + (bit_4);
}

// Scrolling configuration 滚动配置
int scrollPosition = 0;  // Current scroll position 当前滚动位置
const int scrollSpeed = 0;  // Scroll speed (pixels per frame) 滚动速度（每帧像素数）

// Display timing configuration 显示时间配置
unsigned long displayTime = 100;  // Display duration for each image 每个图像显示时长
unsigned long lastImageChangeTime = 0;  // Last image change timestamp 上次图像更改时间戳
int currentImageIndex = 0;  // Current image index 当前图像索引

// Display image function 显示图像函数
void displayimage(int image_no, int array_index){
    // Get image data from array 从数组获取图像数据
    const unsigned long* currentImage = epd_bitmap_allArray[array_index][image_no];
    int IMAGE_WIDTH_CURRENT = IMAGE_WIDTH; 
    int IMAGE_HEIGHT_CURRENT = IMAGE_HEIGHT;
    
    // Draw image pixel by pixel 逐像素绘制图像
    for (int x = 0; x < PANE_WIDTH; x++) {
        for (int y = 0; y < PANE_HEIGHT; y++) {
            // Calculate pixel position 计算像素位置
            int imgX = (scrollPosition + y) % IMAGE_WIDTH_CURRENT;
            int imgY = IMAGE_HEIGHT_CURRENT - 1 - x;
            int index = imgY * IMAGE_WIDTH_CURRENT + imgX;
            
            // Extract color components 提取颜色分量
            uint32_t colorData = pgm_read_dword(&(currentImage[index]));
            uint8_t red = (colorData >> 16) & 0xFF;
            uint8_t green = (colorData >> 8) & 0xFF;
            uint8_t blue = colorData & 0xFF;
            
            // Draw pixel 绘制像素
            dma_display->drawPixelRGB888(x, y, red, green, blue);
        }
    }
    delay(50);  // Small delay for stability 短暂延时以保持稳定
}

// Display red light function 显示红灯函数
void displaycolor_red(){
    if(THIS_COLOR == DISPLAY_RED) 
        dma_display -> fillCircle(32, 32, 32, 0xF800);  // Draw red circle 绘制红色圆圈
}

// Display yellow light function 显示黄灯函数
void displaycolor_yellow(){
    if(THIS_COLOR == DISPLAY_YELLOW) 
        dma_display -> fillCircle(32, 32, 32, 0xFFE0);  // Draw yellow circle 绘制黄色圆圈
}

// Display green light function 显示绿灯函数
void displaycolor_green(){
    if(THIS_COLOR != DISPLAY_GREEN)
        dma_display -> fillCircle(32, 32, 32, 0x0400);  // Draw green circle 绘制绿色圆圈
}

// State control variables 状态控制变量
int cur_state = 0;  // Current state 当前状态
int cur_frame = 0;  // Current animation frame 当前动画帧

// Main loop function 主循环函数
void loop() {
    Serial.println(cur_state);  // Debug output 调试输出
    
    // State change detection 状态改变检测
    bool change_flag = false;
    int to_state = new_state();
    
    // Clear screen on state change 状态改变时清屏
    if(to_state != cur_state){
        dma_display->clearScreen();
        change_flag = true;
    }
    
    // State handling 状态处理
    if(!change_flag){
        if(cur_state > CLEAR_ALL){
            // Handle animation states 处理动画状态
            displayimage(cur_frame, cur_state - CLEAR_ALL - 1);
            cur_frame ++; 
            if(cur_frame >= frames[cur_state - CLEAR_ALL - 1])
                cur_frame = 0;
            delay(34);  // Frame timing delay 帧时序延时
        }
        else{
            // Handle traffic light states 处理交通灯状态
            switch (cur_state) {
                case DISPLAY_RED:
                    displaycolor_red();
                    break;
                case DISPLAY_YELLOW:
                    dma_display -> clearScreen();
                    break;
                case DISPLAY_GREEN:
                    dma_display -> clearScreen();
                    break;
                case CLEAR_ALL:
                    dma_display->clearScreen();
            }
            cur_frame = 0;
        }
    }
    else {
        // Update state on change 状态改变时更新
        cur_state = to_state; 
        cur_frame = 0;
    }
}