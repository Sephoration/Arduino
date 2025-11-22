#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class OLED_Display {
  public:
    // 构造函数
    OLED_Display(uint8_t width = 128, uint8_t height = 64, int8_t resetPin = -1);
    
    // 初始化OLED
    bool begin(uint8_t i2cAddress = 0x3C);
    
    // 显示方法
    void showBootScreen();
    void showMainDisplay(unsigned long seconds);
    void clear();
    void update();
    
    // 状态检查
    bool isInitialized() { return _initialized; }
    
  private:
    Adafruit_SSD1306 _display;
    bool _initialized = false;
    uint8_t _width;
    uint8_t _height;
    
    // 私有工具方法
    void setTextSize(uint8_t size);
    void setTextColor(uint16_t color);
    void setCursor(int16_t x, int16_t y);
    void print(const String &text);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
};

#endif