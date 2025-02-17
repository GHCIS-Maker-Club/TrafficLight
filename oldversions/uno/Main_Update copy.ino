#include <Wire.h>

// I2C addresses for ESP32s
#define ESP32_1_ADDR 0x10
#define ESP32_2_ADDR 0x11
#define ESP32_3_ADDR 0x12

// Commands
#define CMD_GREEN 1
#define CMD_YELLOW 2
#define CMD_RED 3
#define CMD_CLEAR 4

// Timing constants
#define GREEN_TIME 10000    // 10 seconds
#define RED_TIME 20000     // 20 seconds
#define BLINK_INTERVAL 500 // 500ms per blink
#define BLINK_COUNT 3      // Number of blinks

unsigned long sequenceStartTime = 0;
int currentStep = 0;
bool isBlinking = false;
int blinkCount = 0;
unsigned long lastBlinkTime = 0;

void setup() {
  Wire.begin(); // Initialize I2C as master
  Serial.begin(9600);
  delay(1000);
  Serial.println("\n=== Traffic Light Controller Started ===");
}

void sendCommand(byte address, byte command) {
  Wire.beginTransmission(address);
  Wire.write(command);
  byte error = Wire.endTransmission();
  
  Serial.print("Sent command ");
  Serial.print(command);
  Serial.print(" to address 0x");
  Serial.print(address, HEX);
  if (error == 0) {
    Serial.println(" - OK");
  } else {
    Serial.print(" - Error ");
    Serial.println(error);
  }
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - sequenceStartTime >= getStepDuration(currentStep)) {
    currentStep = (currentStep + 1) % 6; // 6 steps in total
    sequenceStartTime = currentTime;
    isBlinking = false;
    blinkCount = 0;
    
    Serial.print("\n=== Step Changed to ");
    Serial.print(currentStep);
    Serial.println(" ===");
  }

  switch (currentStep) {
    case 0: // Green light on ESP32_1 for 10 seconds
      if (currentTime - sequenceStartTime == 0) {
        sendCommand(ESP32_1_ADDR, CMD_GREEN);
      }
      break;

    case 1: // Green light blinking on ESP32_1
      handleBlinking(ESP32_1_ADDR, CMD_GREEN);
      break;

    case 2: // Clear ESP32_1
      if (currentTime - sequenceStartTime == 0) {
        sendCommand(ESP32_1_ADDR, CMD_CLEAR);
      }
      break;

    case 3: // Yellow light blinking on ESP32_2
      handleBlinking(ESP32_2_ADDR, CMD_YELLOW);
      break;

    case 4: // Clear ESP32_2
      if (currentTime - sequenceStartTime == 0) {
        sendCommand(ESP32_2_ADDR, CMD_CLEAR);
      }
      break;

    case 5: // Red light on ESP32_3 for 20 seconds
      if (currentTime - sequenceStartTime == 0) {
        sendCommand(ESP32_3_ADDR, CMD_RED);
      }
      break;
  }
}

unsigned long getStepDuration(int step) {
  switch (step) {
    case 0: return GREEN_TIME;  // Green solid
    case 1: return BLINK_INTERVAL * BLINK_COUNT * 2;  // Green blink
    case 2: return 100;  // Clear ESP32_1
    case 3: return BLINK_INTERVAL * BLINK_COUNT * 2;  // Yellow blink
    case 4: return 100;  // Clear ESP32_2
    case 5: return RED_TIME;  // Red solid
    default: return 0;
  }
}

void handleBlinking(byte address, byte command) {
  unsigned long currentTime = millis();
  
  if (currentTime - lastBlinkTime >= BLINK_INTERVAL) {
    if (blinkCount < BLINK_COUNT * 2) {
      if (blinkCount % 2 == 0) {
        sendCommand(address, command);
      } else {
        sendCommand(address, CMD_CLEAR);
      }
      blinkCount++;
      lastBlinkTime = currentTime;
    }
  }
}