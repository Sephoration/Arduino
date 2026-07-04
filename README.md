# Arduino Projects

Arduino 嵌入式项目集合，主要面向 Nano / UNO / STM32 兼容开发板与 SSD1306 OLED 显示屏，包含应用框架、传感器工具箱、专注计时器和小游戏掌机示例。

## 项目列表

| 项目 | 说明 |
|------|------|
| AppFrame | 嵌入式应用框架，支持应用注册、动态菜单和模块化页面 |
| Focus_Cube | 专注计时器，支持倒计时、环境检测和蜂鸣器反馈 |
| Tool_Box | 传感器工具箱，包含水位、摇杆、烟雾、超声波和光敏模块 |
| Game_Box | OLED 小游戏掌机，包含贪吃蛇、俄罗斯方块和打砖块 |

## 硬件要求

- Arduino Nano / UNO 或兼容开发板
- SSD1306 OLED 0.96 寸 128x64 I2C 显示屏
- 按键、蜂鸣器及各子项目所需传感器模块

## 依赖库

- Adafruit GFX Library
- Adafruit SSD1306
- Adafruit BusIO

项目已包含常用依赖库源码，也可以通过 Arduino IDE 的库管理器安装最新版本。

## 快速开始

1. 使用 Arduino IDE 打开对应子目录下的 `.ino` 文件。
2. 根据实际开发板选择 Board 和 Port。
3. 确认依赖库可用。
4. 按对应项目的引脚定义完成接线。
5. 编译并上传到开发板。

## 目录结构

```text
Arduino/
├── AppFrame/      # 应用框架
├── Focus_Cube/    # 专注计时器
├── Tool_Box/      # 传感器工具箱
├── Game_Box/      # 游戏掌机
└── libraries/     # 第三方依赖库
```

## OLED 接线

| OLED | Arduino |
|------|---------|
| VCC | 3.3V / 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

其他模块的引脚定义请查看各项目中的 `PinDefines.h` 或主程序文件。

## 说明

本仓库用于 Arduino 嵌入式功能实验、模块封装和小型交互设备原型开发。