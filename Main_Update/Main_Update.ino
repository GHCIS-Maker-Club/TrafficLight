#define PIN_1_1 6
#define PIN_1_2 7
#define PIN_1_3 8
#define PIN_1_4 9
#define PIN_2_1 10
#define PIN_2_2 11
#define PIN_2_3 12
#define PIN_2_4 13
#define PIN_3_1 14
#define PIN_3_2 15
#define PIN_3_3 16
#define PIN_3_4 17
int spkPin = A0; // 扬声器连接到模拟引脚 A0
int buttonPin = 22; // 按钮连接到数字引脚 22
int Idletime = 10;
int volume = 128;  // 音量控制 (0-255)
int tones[] = {
  3465, 2850, 2333, 1956, 1638, 1380, 1161, 992, 814, 704, 500
}; 

// 状态变量
unsigned long lastPlayTime = 0;     // 上次播放时间
unsigned long cooldownTime = 3000;   // 冷却时间（毫秒）
bool inCooldown = false;           // 是否在冷却状态

// 声音播放状态
unsigned long soundStartTime = 0;   // 当前音调开始时间
int currentTone = 0;               // 当前音调索引
int currentStep = 0;               // 当前步骤
bool isSoundActive = false;        // 是否正在播放声音
enum SoundType {
  IDLE,
  CHIRP,
  WOODPECKER
} currentSound = IDLE;

void playTone(int freq, int duration) {
  tone(spkPin, freq, duration);
  analogWrite(spkPin, volume);  // 设置音量
}

void updateSound() {
  unsigned long currentMillis = millis();
  
  if (!isSoundActive) {
    // 开始新的声音
    soundStartTime = currentMillis;
    isSoundActive = true;
    
    switch(currentSound) {
      case IDLE:
        playTone(973, 25);
        break;
      case CHIRP:
        if (currentTone < 11) {
          playTone(tones[currentTone], 11);
        }
        break;
      case WOODPECKER:
        if (currentStep < 30) {
          playTone(500, 30);
        }
        break;
    }
  } else {
    // 检查是否需要停止当前声音
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
            Serial.println("CHIRP completed, switching to WOODPECKER with cooldown");
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
              Serial.println("WOODPECKER completed, switching to IDLE");
            }
          }
        }
        break;
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(spkPin, OUTPUT);
  pinMode(buttonPin, INPUT);
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
  Serial.println("System initialized - Non-blocking sound version");
}

long long last_press_time;
int cur_animation_signal;
int cur_light_signal;
const int max_signal = 11;

void send_signal(int cur_signal){
    Serial.print("Wrote:");
    Serial.println(cur_signal);
    int bit_1 = cur_signal & 8;
    int bit_2 = cur_signal & 4;
    int bit_3 = cur_signal & 2;
    int bit_4 = cur_signal & 1;
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

void loop() {
    int cur_period;
    int time_period_normal = millis() % 20000;
    Serial.println(time_period_normal);
    if(time_period_normal < 10000)cur_period = 0; //Red light
    else if(time_period_normal < 18000) cur_period = 2; //Green light
    else cur_period = 3; //Yellow

    unsigned long currentMillis = millis();
    
    // 检查冷却状态
    if (inCooldown && (currentMillis - lastPlayTime >= cooldownTime)) {
        inCooldown = false;
        Serial.println("Cooldown ended");
    }
    
    // 读取按钮状态
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
    if (buttonState == 1) {  // 按钮被按下
        Serial.println("Button Pressed");
        if (currentSound == IDLE && !inCooldown) {  // 只在IDLE状态且不在冷却时响应按钮
        Serial.println("Starting CHIRP sequence");
        currentSound = CHIRP;
        currentTone = 0;
        currentStep = 0;
        isSoundActive = false;

        cur_animation_signal = (cur_animation_signal == max_signal ? cur_animation_signal = 4 : cur_animation_signal + 1);
        send_signal(cur_animation_signal);
        last_press_time = millis();
        
        } else if (currentSound == WOODPECKER && !inCooldown) {  // WOODPECKER状态且不在冷却时也可以响应
        Serial.println("Interrupting WOODPECKER, starting CHIRP sequence");
        currentSound = CHIRP;
        currentTone = 0;
        currentStep = 0;
        isSoundActive = false;
        cur_animation_signal = (cur_animation_signal == max_signal ? cur_animation_signal = 4 : cur_animation_signal + 1);
        send_signal(cur_animation_signal);
        last_press_time = millis();
        }
    }

    else {
        if(millis() - last_press_time > 20000){
            send_signal(cur_period);
        }
        else send_signal(cur_animation_signal);
    }
    
    // 更新声音状态
    updateSound();
}