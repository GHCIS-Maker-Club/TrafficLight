#include <Arduino.h>
#include <Wire.h>

#define I2C_SLAVE_ADDR 0x10  // 从机地址，可以改为0x10, 0x11, 0x12
#define SDA_PIN 8
#define SCL_PIN 9

void receiveEvent(int numBytes) {
  if (Wire.available()) {
    byte command = Wire.read();
    Serial.print("Received command: ");
    Serial.println(command);
    
    // 通过串口输出收到的命令对应的动作
    switch (command) {
      case 1:
        Serial.println("GREEN LIGHT ON");
        break;
      
      case 2:
        Serial.println("YELLOW LIGHT ON");
        break;
      
      case 3:
        Serial.println("RED LIGHT ON");
        break;
      
      case 4:
        Serial.println("ALL LIGHTS OFF");
        break;
    }
  }
}

void setup() {
  Serial.begin(9600);
  
  // 初始化I2C从机
  Wire.begin(I2C_SLAVE_ADDR, SDA_PIN, SCL_PIN);
  
  // 设置接收事件回调函数
  Wire.onReceive(receiveEvent);
  
  Serial.println("I2C Slave initialized");
  Serial.print("Slave address: 0x");
  Serial.println(I2C_SLAVE_ADDR, HEX);
}

void loop() {
  // 主循环为空，所有操作在接收事件中处理
  delay(10);
}
