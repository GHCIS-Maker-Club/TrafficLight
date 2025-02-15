#include <Arduino.h>
#include <Wire.h>

#define I2C_SLAVE_ADDR 0x10  // 从机地址，可以改为0x10, 0x11, 0x12
#define SDA_PIN 22  // 使用GPIO22作为SDA
#define SCL_PIN 32  // 使用GPIO32作为SCL

// 当前灯的状态
bool lightState = false;
String currentLight = "NONE";
unsigned long lastReceiveTime = 0;
unsigned long receiveCount = 0;

void receiveEvent(int numBytes) {
  lastReceiveTime = millis();
  receiveCount++;
  
  Serial.print("\n=== I2C Receive Event ===\n");
  Serial.print("Bytes available: ");
  Serial.println(numBytes);
  Serial.print("Total receives: ");
  Serial.println(receiveCount);
  
  if (Wire.available()) {
    byte command = Wire.read();
    Serial.print("Raw command received: 0x");
    Serial.println(command, HEX);
    
    // 更新灯的状态
    switch(command) {
      case 1: // GREEN
        lightState = true;
        currentLight = "GREEN";
        break;
      case 2: // YELLOW
        lightState = true;
        currentLight = "YELLOW";
        break;
      case 3: // RED
        lightState = true;
        currentLight = "RED";
        break;
      case 4: // CLEAR
        lightState = false;
        currentLight = "NONE";
        break;
      default:
        Serial.print("Unknown command: ");
        Serial.println(command);
        return;
    }

    Serial.print("Current light: ");
    Serial.println(currentLight);
    Serial.print("Light state: ");
    Serial.println(lightState ? "ON" : "OFF");
    Serial.println("=========================\n");
  } else {
    Serial.println("No data available in receive event!");
  }
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  Serial.println("\n=== ESP32 I2C Slave Debug Start ===");
  
  // 先停止可能存在的I2C连接
  Wire.end();
  delay(100);
  
  // 配置引脚模式并启用内部上拉
  pinMode(SDA_PIN, INPUT_PULLUP);
  pinMode(SCL_PIN, INPUT_PULLUP);
  
  // 读取引脚状态
  Serial.print("Initial SDA Pin State: ");
  Serial.println(digitalRead(SDA_PIN) ? "HIGH" : "LOW");
  Serial.print("Initial SCL Pin State: ");
  Serial.println(digitalRead(SCL_PIN) ? "HIGH" : "LOW");
  
  delay(100); // 等待引脚状态稳定
  
  // 设置I2C引脚
  bool setPinsResult = Wire.setPins(SDA_PIN, SCL_PIN);
  Serial.print("Set I2C pins result: ");
  Serial.println(setPinsResult ? "OK" : "FAILED");
  
  // 初始化I2C从机
  Wire.begin(I2C_SLAVE_ADDR, SDA_PIN, SCL_PIN, 100000); // 降低速度到100kHz
  Wire.onReceive(receiveEvent);
  
  delay(100); // 等待初始化完成
  
  // 再次读取引脚状态
  Serial.print("After init SDA Pin State: ");
  Serial.println(digitalRead(SDA_PIN) ? "HIGH" : "LOW");
  Serial.print("After init SCL Pin State: ");
  Serial.println(digitalRead(SCL_PIN) ? "HIGH" : "LOW");
  
  Serial.print("Device Address: 0x");
  Serial.println(I2C_SLAVE_ADDR, HEX);
  Serial.println("I2C Slave initialized");
  Serial.println("Waiting for commands...\n");
}

void loop() {
  static unsigned long lastStatusTime = 0;
  unsigned long currentTime = millis();
  
  // 每5秒打印一次状态
  if (currentTime - lastStatusTime > 5000) {
    lastStatusTime = currentTime;
    Serial.println("\n=== Status Update ===");
    Serial.print("Current light: ");
    Serial.println(currentLight);
    Serial.print("Light state: ");
    Serial.println(lightState ? "ON" : "OFF");
    Serial.print("Time since last receive: ");
    Serial.print((currentTime - lastReceiveTime) / 1000);
    Serial.println(" seconds");
    Serial.print("Total receives: ");
    Serial.println(receiveCount);
    
    // 监控引脚状态
    Serial.print("SDA Pin State: ");
    Serial.println(digitalRead(SDA_PIN) ? "HIGH" : "LOW");
    Serial.print("SCL Pin State: ");
    Serial.println(digitalRead(SCL_PIN) ? "HIGH" : "LOW");
    Serial.println("===================\n");
  }
  delay(10);
}
