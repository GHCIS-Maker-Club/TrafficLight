int spkPin = 12; // 扬声器连接到数字引脚 8
int Idletime = 10;
int tones[] = {
  3465, 2850, 2333, 1956, 1638, 1380, 1161, 992, 814, 704, 500
}; 



void playIdle() {
  for (int a = 0; a <= 5; a++) {
    delay(1800);
    tone(spkPin, 973, 25);
    delay(25);
    noTone(spkPin);
    
  }
}
void playChirp() {
  // Iterate through all tones
  for (int i = 0; i < 11; i++) {
    // Play the next tone
    tone(spkPin, tones[i], 11);
    delay(11);
  }
}
void playWoodpecker() {
  // 17x = 2secs; 85x = 10 seconds
  for (int i = 0; i <= 30; i++) {
    tone(spkPin, 500, 30);
    delay(30);
    noTone(spkPin);
    delay(117); // wait
  }
}


void setup() {
  pinMode(spkPin, OUTPUT); // 设置扬声器引脚为输出模式
}

void loop() {
  // 示例：依次播放三种声音
  playIdle();
  
  playChirp();
  
  playWoodpecker();
  //delay(1000);
}
