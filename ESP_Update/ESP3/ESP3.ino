//---PIN DEFINITION---
//Change THIS_COLOR
//Change switch statement
#define R1 25
#define G1 26
#define BL1 27
#define R2 14
#define G2 12
#define BL2 13
#define CH_A 23
#define CH_B 19
#define CH_C 5
#define CH_D 17
#define CH_E 18 // assign to any available pin if using two panels or 64x64 panels with 1/32 scan
#define CLK 16
#define LAT 4
#define OE 15

#define STATES_PIN_1 2
#define STATES_PIN_2 21
#define STATES_PIN_3 22
#define STATES_PIN_4 33
#define DISPLAY_RED 0
#define DISPLAY_YELLOW 1
#define DISPLAY_GREEN 2
#define CLEAR_ALL 3
#define ANIMATION_1 4
#define ANIMATION_2 5
#define ANIMATION_3 6
#define ANIMATION_4 7
#define ANIMATION_5 8
#define ANIMATION_6 9
#define ANIMATION_7 10
#define ANIMATINO_8 11
#define THIS_COLOR 2 //GREEN
//Other configuration

#include "ESP32-HUB75-MatrixPanel-I2S-DMA.h"
#include <FastLED.h>
#include "Image.h"
// Configure for your panel(s) as appropriate!
#define PANEL_WIDTH 64
#define PANEL_HEIGHT 64  	
#define PANELS_NUMBER 1 	// Number of chained panels
#define PIN_E 18

#define PANE_WIDTH PANEL_WIDTH * PANELS_NUMBER
#define PANE_HEIGHT PANEL_HEIGHT

const int IMAGE_WIDTH = 64; // The width of your image
const int IMAGE_HEIGHT = 64; // The height of your image
// placeholder for the matrix object
MatrixPanel_I2S_DMA *dma_display = nullptr;
uint16_t time_counter = 0, cycles = 0, fps = 0;
unsigned long fps_timer;
uint8_t brightness = 255;



void setup() {
    pinMode(STATES_PIN_1, INPUT_PULLDOWN);
    pinMode(STATES_PIN_2, INPUT_PULLDOWN);
    pinMode(STATES_PIN_3, INPUT_PULLDOWN);
    pinMode(STATES_PIN_4, INPUT_PULLDOWN);

    //Initialize the Matrix LED

    Serial.begin(9600);
    
    Serial.println(F("*****************************************************"));
    Serial.println(F("*        ESP32 PICTURE DISPLAY        *"));
    Serial.println(F("*****************************************************"));
    
    /*
        Two 64x64 panels chained, so we the setup is customized like this
    */
    HUB75_I2S_CFG mxconfig;
    mxconfig.mx_height = PANEL_HEIGHT;     
    mxconfig.chain_length = PANELS_NUMBER; 
    mxconfig.gpio.e = PIN_E;                
    mxconfig.driver = HUB75_I2S_CFG::FM6124;     

    dma_display = new MatrixPanel_I2S_DMA(mxconfig);
    dma_display->setBrightness8(brightness);   
    if( not dma_display->begin() )
        Serial.println("****** !KABOOM! I2S memory allocation failed ***********");
    
    Serial.println("Fill screen: RED");
    //dma_display->fillScreenRGB888(255, 0, 0);
    delay(100);
    Serial.println("Starting image display...");
    fps_timer = millis();

    dma_display -> setBrightness8(128);

}

int cur_gif;
const int max_gif = 10;

int new_state(){
    int bit_1 = digitalRead(STATES_PIN_1);
    int bit_2 = digitalRead(STATES_PIN_2);
    int bit_3 = digitalRead(STATES_PIN_3);
    int bit_4 = digitalRead(STATES_PIN_4);
    return (bit_1 << 3) + (bit_2 << 2) + (bit_3 << 1) + (bit_4);
}
int scrollPosition = 0;  // Starting position for scrolling
const int scrollSpeed = 0;  // Number of rows to scroll per frame
unsigned long displayTime = 100; // Time to display each image in milliseconds
unsigned long lastImageChangeTime = 0; // Last time the image was changed
int currentImageIndex = 0;

void displayimage(int image_no, int array_index){
    const unsigned long* currentImage = epd_bitmap_allArray[array_index][image_no];
    int IMAGE_WIDTH_CURRENT = IMAGE_WIDTH; 
    int IMAGE_HEIGHT_CURRENT = IMAGE_HEIGHT;
    for (int x = 0; x < PANE_WIDTH; x++) {
        for (int y = 0; y < PANE_HEIGHT; y++) {
            int imgX = (scrollPosition + y) % IMAGE_WIDTH_CURRENT;
            int imgY = IMAGE_HEIGHT_CURRENT - 1 - x;
            int index = imgY * IMAGE_WIDTH_CURRENT + imgX;
            uint32_t colorData = pgm_read_dword(&(currentImage[index]));
            uint8_t red = (colorData >> 16) & 0xFF;
            uint8_t green = (colorData >> 8) & 0xFF;
            uint8_t blue = colorData & 0xFF;
            dma_display->drawPixelRGB888(x, y, red, green, blue);
        }
    }
    delay(50);
}
void displaycolor_red(){
    if(THIS_COLOR == DISPLAY_RED) dma_display -> fillCircle(32, 32, 32, 0xF800);
}
void displaycolor_yellow(){
    if(THIS_COLOR == DISPLAY_YELLOW) dma_display -> fillCircle(32, 32, 32, 0xFFE0);
}
void displaycolor_green(){
    if(THIS_COLOR == DISPLAY_GREEN)dma_display -> fillCircle(32, 32, 32, 0x0400);
}
int cur_state = 0;
int cur_frame = 0;
void loop() {
    Serial.println(cur_state);
    bool change_flag = false;
    int to_state = new_state();
    //Debug
    //int to_state = Serial.parseInt();
    //int to_state = 8;

    if(to_state != cur_state){
        dma_display->clearScreen();
        change_flag = true;
    }
    if(!change_flag){
        if(cur_state > CLEAR_ALL){
            displayimage(cur_frame, cur_state - CLEAR_ALL - 1);
            cur_frame ++; 
            if(cur_frame >= frames[cur_state - CLEAR_ALL - 1])cur_frame = 0;
            delay(34);
        }
        else{
            switch (cur_state) {
                case 0:
                    dma_display -> clearScreen();
                    break;
                case 2:
                    displaycolor_green();
                    //dma_display -> clearScreen();
                    break;
                case 3:
                    dma_display->clearScreen();
                    //displaycolor_yellow();//BUG1
                    break;
            }
            cur_frame = 0;
        }
    }
    else {
        cur_state = to_state; cur_frame = 0;
    }
}