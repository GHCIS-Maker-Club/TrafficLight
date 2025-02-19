#include "DFRobot_RGBLCD1602.h"

// Pin Definitions for Signal Groups
// 信号组引脚定义
#define PIN_1_1 6    // First group, bit 1 / 第一组，位1
#define PIN_1_2 7    // First group, bit 2 / 第一组，位2
#define PIN_1_3 8    // First group, bit 3 / 第一组，位3
#define PIN_1_4 9    // First group, bit 4 / 第一组，位4
#define PIN_2_1 10   // Second group, bit 1 / 第二组，位1
#define PIN_2_2 11   // Second group, bit 2 / 第二组，位2
#define PIN_2_3 12   // Second group, bit 3 / 第二组，位3
#define PIN_2_4 13   // Second group, bit 4 / 第二组，位4
#define PIN_3_1 14   // Third group, bit 1 / 第三组，位1
#define PIN_3_2 15   // Third group, bit 2 / 第三组，位2
#define PIN_3_3 16   // Third group, bit 3 / 第三组，位3
#define PIN_3_4 17   // Third group, bit 4 / 第三组，位4

// LCD Configuration
DFRobot_RGBLCD1602 lcd(0x6B, 16, 2);  // 16 characters and 2 lines of show

// Hardware Configuration
// 硬件配置
int spkPin = A0;     // Speaker connected to analog pin A0 / 扬声器连接到模拟引脚 A0
int buttonPin = 22;  // Button connected to digital pin 22 / 按钮连接到数字引脚 22
int Idletime = 10;   // Idle time duration / 空闲时间持续时间
int volume = 128;    // Volume control (0-255) / 音量控制 (0-255)

// Tone frequencies for sound effects
// 声音效果的音调频率
int tones[] = {
  3465, 2850, 2333, 1956, 1638, 1380, 1161, 992, 814, 704, 500
}; 

// State Variables
// 状态变量
unsigned long lastPlayTime = 0;     // Last sound play time / 上次播放声音的时间
unsigned long cooldownTime = 1000;  // Cooldown duration in milliseconds / 冷却时间（毫秒）
bool inCooldown = false;           // Cooldown status flag / 是否在冷却状态

// Sound Playback Status
// 声音播放状态
unsigned long soundStartTime = 0;   // Current tone start time / 当前音调开始时间
int currentTone = 0;               // Current tone index / 当前音调索引
int currentStep = 0;               // Current step in sequence / 当前步骤
bool isSoundActive = false;        // Sound playback status / 是否正在播放声音

// Sound Type Enumeration
// 声音类型枚举
enum SoundType {
  IDLE,         // Idle state sound / 空闲状态声音
  CHIRP,        // Bird chirp sound / 鸟叫声
  WOODPECKER    // Woodpecker sound / 啄木鸟声音
} currentSound = IDLE;

// Variables for button and animation control
// 按钮和动画控制变量
long long last_press_time = 0;        // Last button press time / 最后一次按钮按下时间
int cur_animation_signal = 0;         // Current animation signal / 当前动画信号
int cur_light_signal = 0;            // Current traffic light signal / 当前交通灯信号
const int max_signal = 11;       // Maximum signal value / 最大信号值
int last_period = -1;            // Last traffic light period / 上一次的交通灯状态
int button_count = 0;            // Button press counter

// Play a tone with specified frequency and duration
// 播放指定频率和持续时间的音调
void playTone(int freq, int duration) {
  tone(spkPin, freq, duration);
  analogWrite(spkPin, volume);  // Set volume / 设置音量
}

// Update sound state and playback
// 更新声音状态和播放
void updateSound() {
  unsigned long currentMillis = millis();
  
  if (!isSoundActive) {
    // Start new sound / 开始新的声音
    soundStartTime = currentMillis;
    isSoundActive = true;
    
    switch(currentSound) {
      case IDLE:
        //playTone(973, 25);  // Play idle tone. Default is off. / 播放空闲音调。默认关闭。
        break;
      case CHIRP:
        if (currentTone < 11) {
          playTone(tones[currentTone], 11);  // Play chirp sequence / 播放鸟叫序列
        }
        break;
      case WOODPECKER:
        if (currentStep < 30) {
          playTone(500, 30);  // Play woodpecker sound / 播放啄木鸟声音
        }
        break;
    }
  } else {
    // Check if current sound should stop
    // 检查当前声音是否应该停止
    unsigned long elapsed = currentMillis - soundStartTime;
    
    switch(currentSound) {
      case IDLE:
        if (elapsed >= 25) {
          noTone(spkPin);
          if (elapsed >= 1825) {
            isSoundActive = false;
          }
        }
        break;
        
      case CHIRP:
        if (elapsed >= 11) {
          noTone(spkPin);
          currentTone++;
          isSoundActive = false;
          if (currentTone >= 11) {
            currentTone = 0;
            currentSound = WOODPECKER;
            inCooldown = true;
            lastPlayTime = currentMillis;
          }
        }
        break;
        
      case WOODPECKER:
        if (elapsed >= 30) {
          noTone(spkPin);
          if (elapsed >= 147) {
            currentStep++;
            isSoundActive = false;
            if (currentStep >= 30) {
              currentStep = 0;
              currentSound = IDLE;
              inCooldown = true;
              lastPlayTime = currentMillis;
            }
          }
        }
        break;
    }
  }
}
// Send signal to all three groups
// 向所有三组发送信号
void send_signal(int cur_signal){
    // Convert signal to binary bits / 将信号转换为二进制位
    int bit_1 = cur_signal & 8;  // Most significant bit / 最高位
    int bit_2 = cur_signal & 4;  // Second bit / 次高位
    int bit_3 = cur_signal & 2;  // Third bit / 次低位
    int bit_4 = cur_signal & 1;  // Least significant bit / 最低位
    
    // Send to all three groups / 发送到所有三组
    digitalWrite(PIN_1_1, bit_1);
    digitalWrite(PIN_1_2, bit_2);
    digitalWrite(PIN_1_3, bit_3);
    digitalWrite(PIN_1_4, bit_4);
    digitalWrite(PIN_2_1, bit_1);
    digitalWrite(PIN_2_2, bit_2);
    digitalWrite(PIN_2_3, bit_3);
    digitalWrite(PIN_2_4, bit_4);
    digitalWrite(PIN_3_1, bit_1);
    digitalWrite(PIN_3_2, bit_2);
    digitalWrite(PIN_3_3, bit_3);
    digitalWrite(PIN_3_4, bit_4);
}
// Initialize system
// 系统初始化
void setup() {
  Serial.begin(9600);  // Initialize serial communication / 初始化串口通信
       
  // Initialize LCD
  lcd.init();
  lcd.setPWM(lcd.REG_ONLY, 255);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Traffic Light");
  lcd.setCursor(0, 1);
  lcd.print("Maker Club");
  delay(2000);  // Show welcome message for 2 seconds
  lcd.clear();
  
  // Play welcome sound
  playTone(1956, 100);  // 高音
  delay(100);
  playTone(2333, 100);  // 更高音
  delay(100);
  playTone(2850, 200);  // 最高音
  delay(200);
  noTone(spkPin);
  
  // 快速闪烁序列
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Check");
  lcd.setCursor(0, 1);
  lcd.print("Testing Lights");
  
  // 快速闪烁10次，每种颜色闪烁200ms
  for(int i = 0; i < 2; i++) {
    // 红灯
    send_signal(0);  // 红灯
    delay(100);
    send_signal(8);  // 关闭

    
    // 绿灯
    send_signal(1);  // 绿灯
    delay(100);
    send_signal(8);  // 关闭

    
    // 黄灯
    send_signal(2);  // 黄灯
    delay(100);
    send_signal(8);  // 关闭
    delay(50);
  }
  lcd.clear();
  //循环五次
  for(int i = 0; i < 5; i++){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Starting");
    lcd.setCursor(0, 1);
    lcd.print("please wait");
    cur_animation_signal = (cur_animation_signal == max_signal ? cur_animation_signal = 4 : cur_animation_signal + 1);
    send_signal(cur_animation_signal);
    last_press_time = millis();     
    delay(1000);
  //显示准备就绪
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ready");
  lcd.setCursor(0, 1);
  lcd.print("Press Button");
  }
  // Configure pin modes / 配置引脚模式
  pinMode(spkPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  // Configure signal output pins / 配置信号输出引脚
  pinMode(PIN_1_1, OUTPUT);
  pinMode(PIN_1_2, OUTPUT);
  pinMode(PIN_1_3, OUTPUT);
  pinMode(PIN_1_4, OUTPUT);
  pinMode(PIN_2_1, OUTPUT);
  pinMode(PIN_2_2, OUTPUT);
  pinMode(PIN_2_3, OUTPUT);
  pinMode(PIN_2_4, OUTPUT);
  pinMode(PIN_3_1, OUTPUT);
  pinMode(PIN_3_2, OUTPUT);
  pinMode(PIN_3_3, OUTPUT);
  pinMode(PIN_3_4, OUTPUT);
}

// Main program loop
// 主程序循环
void loop() {
    // Calculate traffic light period / 计算交通灯周期
    int cur_period;
    int time_period_normal = millis() % 20000;
    
    // Determine traffic light state and calculate countdown
    if(time_period_normal < 10000) {
        cur_period = 0;           // Red light
        int countdown = (10000 - time_period_normal) / 1000;  // Convert to seconds
    } else if(time_period_normal < 18000) {
        cur_period = 2;     // Green light
        int countdown = (18000 - time_period_normal) / 1000;  // Convert to seconds
    } else {
        cur_period = 3;     // Yellow light
        int countdown = (20000 - time_period_normal) / 1000;  // Convert to seconds
    }

    unsigned long currentMillis = millis();
    
    // Check cooldown status / 检查冷却状态
    if (inCooldown && (currentMillis - lastPlayTime >= cooldownTime)) {
        inCooldown = false;
    }
    
    // Read button state / 读取按钮状态
    int buttonState = digitalRead(buttonPin);
    
    // 每100ms打印一次按钮状态
    static unsigned long lastButtonDebugTime = 0;
    if (currentMillis - lastButtonDebugTime >= 100) {
        lastButtonDebugTime = currentMillis;
        //Serial.print("Button: ");
        //Serial.print(buttonState);
        //Serial.print(", Sound: ");
        //Serial.print(currentSound == IDLE ? "IDLE" : (currentSound == CHIRP ? "CHIRP" : "WOODPECKER"));
        //Serial.print(", Step: ");
        //Serial.println(currentSound == CHIRP ? currentTone : currentStep);
    }
    
    // 处理按钮按下
    if (buttonState == 1) {  // Button is pressed / 按钮被按下
        Serial.println("Button Pressed");
        if (currentSound == IDLE && !inCooldown) {  // Only respond in IDLE state and not in cooldown / 只在空闲状态且不在冷却时响应按钮
            Serial.println("Starting CHIRP sequence");
            currentSound = CHIRP;                    // Switch to CHIRP sound mode / 切换到鸟叫声模式
            currentTone = 0;                        // Reset tone counter / 重置音调计数器
            currentStep = 0;                        // Reset step counter / 重置步骤计数器
            isSoundActive = false;                  // Ready for new sound / 准备播放新声音

            button_count++;
            // Update LCD display
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Button Pressed");
            lcd.setCursor(0, 1);
            lcd.print("Count: " + String(button_count));

            // Update and send animation signal / 更新并发送动画信号
            cur_animation_signal = (cur_animation_signal == max_signal ? cur_animation_signal = 4 : cur_animation_signal + 1);
            send_signal(cur_animation_signal);
            last_press_time = millis();            // Record press time / 记录按压时间
        
        } else if (currentSound == WOODPECKER && !inCooldown) {  // Can also respond during WOODPECKER state if not in cooldown / WOODPECKER状态且不在冷却时也可以响应
            Serial.println("Interrupting WOODPECKER, starting CHIRP sequence");
            currentSound = CHIRP;                    // Switch to CHIRP sound mode / 切换到鸟叫声模式
            currentTone = 0;                        // Reset tone counter / 重置音调计数器
            currentStep = 0;                        // Reset step counter / 重置步骤计数器
            isSoundActive = false;                  // Ready for new sound / 准备播放新声音


            button_count++;
            // Update LCD display
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Button Pressed");
            lcd.setCursor(0, 1);
            lcd.print("Count: " + String(button_count));

            cur_animation_signal = (cur_animation_signal == max_signal ? cur_animation_signal = 4 : cur_animation_signal + 1);
            send_signal(cur_animation_signal);
            last_press_time = millis();            // Record press time / 记录按压时间
        }
    }
    else {
        // Return to normal traffic light after 20 seconds
        // 20秒后返回正常交通灯显示
        if(millis() - last_press_time > 20000){
            send_signal(cur_period);
            
            // Update LCD display every 500ms
            static unsigned long last_display_update = 0;
            if (millis() - last_display_update >= 500) {  // Reduced from 100ms to 500ms
                last_display_update = millis();
                
                // Only clear screen when state changes
                if (cur_period != last_period) {
                    lcd.clear();
                    if(cur_period == 0) {
                        lcd.setCursor(0, 0);
                        lcd.print("State: RED");
                        lcd.setCursor(0, 1);
                        lcd.print("Time Left: ");
                    } else if(cur_period == 2) {
                        lcd.setCursor(0, 0);
                        lcd.print("State: GREEN");
                        lcd.setCursor(0, 1);
                        lcd.print("Time Left: ");
                    } else {
                        lcd.setCursor(0, 0);
                        lcd.print("State: YELLOW");
                        lcd.setCursor(0, 1);
                        lcd.print("Time Left: ");
                    }
                }
                
                // Update only the countdown numbers
                lcd.setCursor(10, 1);  // Position after "Time Left: "
                if(cur_period == 0) {
                    lcd.print(String((10000 - time_period_normal) / 1000) + "s   ");
                } else if(cur_period == 2) {
                    lcd.print(String((18000 - time_period_normal) / 1000) + "s   ");
                } else {
                    lcd.print(String((20000 - time_period_normal) / 1000) + "s   ");
                }
                
                last_period = cur_period;
            }
        }
        else {
            send_signal(cur_animation_signal);
            last_period = -1;  // Reset last_period when in animation mode
        }
    }
    
    // Update sound system / 更新声音系统
    updateSound();
}