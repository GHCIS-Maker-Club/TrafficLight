int spkPin = 12; // 扬声器连接到数字引脚 8
int buttonPin = 11; // 按钮连接到数字引脚 11
int Idletime = 10;
int tones[] = {
  3465, 2850, 2333, 1956, 1638, 1380, 1161, 992, 814, 704, 500
}; 

// 状态变量
unsigned long lastPlayTime = 0;     // 上次播放时间
unsigned long cooldownTime = 100;   // 冷却时间（毫秒）
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

void updateSound() {
  unsigned long currentMillis = millis();
  
  if (!isSoundActive) {
    // 开始新的声音
    soundStartTime = currentMillis;
    isSoundActive = true;
    
    switch(currentSound) {
      case IDLE:
        tone(spkPin, 973, 25);
        break;
      case CHIRP:
        if (currentTone < 11) {
          tone(spkPin, tones[currentTone], 11);
        }
        break;
      case WOODPECKER:
        if (currentStep < 30) {
          tone(spkPin, 500, 30);
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
            Serial.println("CHIRP completed, switching to WOODPECKER");
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
  Serial.println("System initialized - Non-blocking sound version");
}

void loop() {
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
    Serial.print("Button: ");
    Serial.print(buttonState);
    Serial.print(", Sound: ");
    Serial.print(currentSound == IDLE ? "IDLE" : (currentSound == CHIRP ? "CHIRP" : "WOODPECKER"));
    Serial.print(", Step: ");
    Serial.println(currentSound == CHIRP ? currentTone : currentStep);
  }
  
  // 处理按钮按下
  if (buttonState == 1) {
    if (!inCooldown && (currentSound == IDLE || currentSound == WOODPECKER)) {  // 只在IDLE或WOODPECKER状态响应
      Serial.println("Starting sound sequence");
      currentSound = CHIRP;
      currentTone = 0;
      currentStep = 0;
      isSoundActive = false;
    } else if (inCooldown) {
      Serial.println("Cooldown active - Please wait");
    } else if (currentSound == CHIRP) {
      Serial.println("CHIRP in progress - ignoring button");
    }
  }
  
  // 更新声音状态
  updateSound();
}
