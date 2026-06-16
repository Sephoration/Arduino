# Arduino Projects

Arduino 嵌入式项目集合，基于 Nano/UNO + SSD1306 OLED。

## 项目列表

| 项目 | 说明 |
|------|------|
| **AppFrame** | 嵌入式应用框架，自动注册 + 动态菜单 + 模块化架构 |
| **Focus Cube** | 专注计时器，5-60min 倒计时 + 环境检测（光线/噪音/人体） |
| **Tool Box** | 传感器工具箱：水位 / 摇杆 / 烟雾 / 超声波 / 光敏 |
| **Game Box** | 三合一游戏掌机：贪吃蛇 / 俄罗斯方块 / 打砖块，EEPROM 存档 + 蜂鸣器音效 |

## 硬件要求

- Arduino Nano / UNO
- SSD1306 OLED 0.96寸 128x64 (I2C)

## 依赖库

- Adafruit GFX Library
- Adafruit SSD1306

## 快速开始

1. 安装 [Arduino IDE](https://www.arduino.cc/en/software)
2. 安装依赖库（库管理器搜索 Adafruit GFX / SSD1306）
3. 打开对应 `.ino` 文件，编译上传

## 目录结构

```
Arduino/
├── AppFrame/      # 应用框架
├── Focus_Cube/    # 专注计时器
├── Tool_Box/      # 传感器工具箱
├── Game_Box/      # 游戏掌机
└── libraries/     # 依赖库
```

## 接线（OLED）

| OLED | Arduino |
|------|---------|
| VCC | 3.3V / 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

其他传感器引脚见各项目 `PinDefines.h`。

## 许可证

仅供学习研究使用。
