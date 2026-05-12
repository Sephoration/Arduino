# 🤖 Arduino Projects

Arduino 嵌入式项目集合。

## 📦 项目列表

### 1. 🖼️ AppFrame
嵌入式应用框架，支持自动应用注册和动态菜单生成。

**特性:
- 自动应用注册机制
- 动态菜单生成
- 模块化架构
- OLED 显示驱动

**目录**: `AppFrame/`

---

### 2. ⏱️ Focus Cube
专注学习计时器，带环境检测功能。

**功能:
- 倒计时功能（5-60分钟可调）
- 环境检测（光线、噪音、人体）
- 蜂鸣器提醒
- OLED 显示

**目录**: `Focus_Cube/`

---

### 3. 🛠️ Tool Box
传感器工具箱，多种传感器演示应用。

**包含传感器**:
- 水位检测
- 摇杆传感器
- 烟雾传感器
- 超声波测距
- 光敏传感器

**目录**: `Tool_Box/`

---

## 🔧 硬件要求

- 开发板: Arduino Nano / UNO
- 显示: SSD1306 OLED (0.96寸 128x64)
- 其他: 根据具体项目而定

## 📚 依赖库

- Adafruit GFX Library
- Adafruit SSD1306

## 🚀 快速开始

### 1. 安装 Arduino IDE
下载并安装 [Arduino IDE](https://www.arduino.cc/en/software)

### 2. 安装依赖库
在 Arduino IDE 库管理器中搜索并安装：
- Adafruit GFX Library
- Adafruit SSD1306

### 3. 打开项目
在 Arduino IDE 中打开对应项目的 `.ino` 文件。

### 4. 编译上传
选择正确的开发板和端口，编译并上传。

## 📂 目录结构

```
Arduino/
├── AppFrame/           # 应用框架
├── Focus_Cube/         # 专注计时器
├── Tool_Box/          # 传感器工具箱
└── libraries/          # 依赖库
```

## 📝 项目说明

### AppFrame
通用应用框架，提供：
- 应用注册机制
- OLED 显示驱动
- 主页和列表页面模板

### Focus Cube
专注学习辅助工具：
- 5种状态机
- 可选环境监测
- 多按键操作

### Tool Box
多种传感器演示：
- 水位检测
- 摇杆输入
- 烟雾检测
- 超声波测距
- 光敏传感器

## 🔌 接线说明

### OLED 显示
- VCC → 3.3V 或 5V
- GND → GND
- SDA → A4 (SDA)
- SCL → A5 (SCL)

### 其他传感器
根据具体项目的 `PinDefines.h` 或 `AppFrame.ino` 中的引脚定义连接。

## 🛠️ 开发指南

### 添加新应用（AppFrame）
1. 在 `AppType.h` 中添加应用 ID
2. 声明应用函数
3. 注册应用
4. 创建应用文件

### 配置引脚
修改 `PinDefines.h` 或 `.ino` 文件中的引脚定义。

## 🐛 常见问题

**编译错误**
- 检查库是否正确安装
- 确认开发板选择正确

**显示异常**
- 检查 OLED 地址（0x3C 或 0x3D
- 确认接线正确

**按键无响应**
- 检查引脚模式
- 确认上拉电阻配置

## 📄 许可证

本项目仅供学习和研究使用。

---

**© 2024 Arduino Projects**
