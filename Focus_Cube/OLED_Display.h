// OLED_Display.h - OLED显示屏控制类
// 封装SSD1306 OLED显示屏的初始化、控制和绘制功能
// 提供简洁的API接口，方便上层应用使用

#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "PinDefines.h"

// OLED显示控制类
class OLED_Display {
public:
    // 构造函数 - 使用PinDefines中的参数
    OLED_Display();
    
    // 初始化显示设备
    bool begin();
    
    // 基础显示控制方法
    void clear();                   // 清空显示缓存
    void update();                  // 更新显示内容
    void showBorder();              // 显示边框
    void showBootScreen();          // 显示启动画面
    
    // 图形绘制工具方法
    void setTextSize(uint8_t size);     // 设置文字大小
    void setTextColor(uint16_t color);  // 设置文字颜色
    void setCursor(int16_t x, int16_t y); // 设置光标位置
    void print(const String &text);     // 打印文本
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color); // 填充矩形
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color); // 绘制矩形边框
    void fillScreen(uint16_t color);    // 填充整个屏幕
    
    // 新增：居中文本显示（方便界面布局）
    void printCentered(int16_t y, const String &text, uint8_t textSize = 1);
    
    // 获取初始化状态
    bool isInitialized() { return _initialized; }
    
private:
    Adafruit_SSD1306 _display;      // SSD1306显示对象
    bool _initialized = false;      // 初始化标志
};

#endif