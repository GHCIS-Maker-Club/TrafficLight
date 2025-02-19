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
  - Main Controller: Arduino Mega 2560
  - Display Controllers: Multiple ESP32 modules
  - LED Matrix Display (16x16)
  - Custom-designed enclosure
  - Sound module integration
  
- **Software Features:**
  - Real-time mode switching
  - Multi-controller synchronization
  - Wireless communication between components
  - Custom animation support
  - Emoji library integration
  - Sound effects support

### Reference Projects
- [traffic-crossing](https://github.com/ronnied/traffic-crossing) - Sound effects reference

### Project Structure
```
TrafficLight/
├── Main_Update/              # Main Arduino controller code
│   └── Main_Update.ino      # Central control logic
├── ESP_Update/              # ESP32 display controller code
│   ├── ESP1/               # First display controller
│   ├── ESP2/               # Second display controller
│   └── ESP3/               # Third display controller
├── oldversions/            # Previous iterations of the project
└── docs/                   # Documentation
```

### Component Overview
1. **Main Controller (Arduino UNO)**
   - Manages overall system state
   - Coordinates display transitions
   - Handles sound effects
   - Controls timing and synchronization

2. **ESP32 Display Controllers**
   - Each ESP32 controls a separate LED matrix
   - Wireless communication with main controller
   - Supports multiple display modes
   - Real-time animation rendering

### Setup Instructions
1. **Main Controller Setup**
   - Upload `Main_Update/Main_Update.ino` to Arduino UNO
   - Connect sound module and other peripherals
   - Verify serial communication

2. **ESP32 Display Controllers Setup**
   - For each ESP32:
     1. Open corresponding folder in ESP_Update
     2. Install required libraries
     3. Configure WiFi settings if needed
     4. Upload code to respective ESP32
     5. Connect LED matrix display

3. **System Integration**
   - Ensure all components are powered appropriately
   - Verify wireless communication between controllers
   - Test synchronization and display modes

### Dependencies
- Arduino IDE
- ESP32 Arduino Core
- FastLED Library
- Adafruit GFX Library
- WiFi Library
- [ESP32-HUB75-MatrixPanel-I2S-DMA](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA)

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
  - 主控制器：Arduino Mega 2560
  - 显示控制器：多个ESP32模块
  - LED点阵显示屏 (16x16)
  - 定制外壳设计
  - 声音模块集成
  
- **软件特性：**
  - 实时模式切换
  - 多控制器同步
  - 组件间无线通信
  - 自定义动画支持
  - 表情符号库集成
  - 声音效果支持

### 参考项目
- [traffic-crossing](https://github.com/ronnied/traffic-crossing) - 声音效果参考

### 项目结构
```
TrafficLight/
├── Main_Update/              # Arduino主控制器代码
│   └── Main_Update.ino      # 中央控制逻辑
├── ESP_Update/              # ESP32显示控制器代码
│   ├── ESP1/               # 第一显示控制器
│   ├── ESP2/               # 第二显示控制器
│   └── ESP3/               # 第三显示控制器
├── oldversions/            # 项目历史版本
└── docs/                   # 文档
```

### 组件概述
1. **主控制器 (Arduino UNO)**
   - 管理整体系统状态
   - 协调显示转换
   - 处理声音效果
   - 控制时序和同步

2. **ESP32显示控制器**
   - 每个ESP32控制一个LED矩阵
   - 与主控制器进行无线通信
   - 支持多种显示模式
   - 实时动画渲染

### 设置说明
1. **主控制器设置**
   - 将 `Main_Update/Main_Update.ino` 上传至Arduino UNO
   - 连接声音模块和其他外设
   - 验证串口通信

2. **ESP32显示控制器设置**
   - 对于每个ESP32：
     1. 打开ESP_Update中对应的文件夹
     2. 安装所需库文件
     3. 根据需要配置WiFi设置
     4. 将代码上传至相应的ESP32
     5. 连接LED点阵显示屏

3. **系统集成**
   - 确保所有组件供电正常
   - 验证控制器间的无线通信
   - 测试同步和显示模式

### 依赖项
- Arduino IDE开发环境
- ESP32 Arduino核心库
- FastLED库
- Adafruit GFX库
- WiFi库
- [ESP32-HUB75-MatrixPanel-I2S-DMA](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA)

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

