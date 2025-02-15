# TrafficLight - Freedom Through Light 自由之光

🚦 An interactive art installation that reimagines the traditional traffic light | 重新想象传统红绿灯的互动艺术装置

[English](#english) | [中文](#chinese)

![License](https://img.shields.io/github/license/GHCIS-Maker-Club/TrafficLight)

## English

### Project Overview
This art installation explores the concept of freedom through the familiar yet transformative medium of a traffic light. Using modern technology to break free from conventional constraints, this installation challenges our perception of urban signals and their role in our daily lives.

### Features
- 🎨 Three display modes:
  - Classic traffic light mode
  - Emoji display mode
  - Animated GIF mode
- 💡 16x16 LED Matrix Display for vibrant visuals
- 🎮 Interactive control system
- 🔄 Smooth transitions between different modes
- 📱 Wireless control capabilities

### Technical Details
- **Hardware Components:**
  - ESP32 microcontroller
  - Arduino integration
  - LED Matrix Display (16x16)
  - Custom-designed enclosure
  
- **Software Features:**
  - Real-time mode switching
  - Wireless communication
  - Custom animation support
  - Emoji library integration

### Reference Projects
- [traffic-crossing](https://github.com/ronnied/traffic-crossing) - An Arduino-based pedestrian traffic crossing lights project with sound effects, providing inspiration for traffic light timing and interaction patterns.

### Installation
1. Clone this repository
2. Install required dependencies
3. Upload the code to your ESP32
4. Connect the LED matrix display
5. Power up and enjoy!

### Dependencies
- ESP32 Arduino Core
- FastLED Library
- Adafruit GFX Library
- WiFi Library
- [ESP32-HUB75-MatrixPanel-I2S-DMA](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA) - For driving the LED matrix display

### Directory Structure
```
TrafficLight/
├── src/
│   ├── main.ino
│   ├── animations/
│   └── emoji/
├── hardware/
│   └── schematics/
├── docs/
└── examples/
```

## License
MIT License - feel free to use and modify for your own projects.

## Contributors
<a href="https://github.com/GHCIS-Maker-Club/TrafficLight/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=GHCIS-Maker-Club/TrafficLight" />
</a>

---

## Chinese

### 项目概述
这个艺术装置通过熟悉却富有创新性的红绿灯媒介来探索自由的概念。运用现代技术打破传统束缚，这个装置挑战了我们对城市信号灯及其在日常生活中角色的认知。

### 功能特点
- 🎨 三种显示模式：
  - 经典红绿灯模式
  - 表情符号显示模式
  - 动图显示模式
- 💡 16x16 LED点阵屏幕展示
- 🎮 交互式控制系统
- 🔄 模式之间流畅切换
- 📱 无线控制功能

### 技术细节
- **硬件组件：**
  - ESP32 微控制器
  - Arduino 集成
  - LED点阵显示屏 (16x16)
  - 定制外壳
  
- **软件特性：**
  - 实时模式切换
  - 无线通信
  - 自定义动画支持
  - 表情符号库集成

### 安装说明
1. 克隆此仓库
2. 安装所需依赖
3. 将代码上传至ESP32
4. 连接LED点阵屏
5. 通电运行

### 依赖项
- ESP32 Arduino Core
- FastLED 库
- Adafruit GFX 库
- WiFi 库

### 目录结构
```
TrafficLight/
├── src/
│   ├── main.ino
│   ├── animations/
│   └── emoji/
├── hardware/
│   └── schematics/
├── docs/
└── examples/
```

## License
MIT License - feel free to use and modify for your own projects.

## Contributors
<a href="https://github.com/GHCIS-Maker-Club/TrafficLight/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=GHCIS-Maker-Club/TrafficLight" />
</a>

