#include "OLED_Display.h"

// 构造函数
OLED_Display::OLED_Display(uint8_t width, uint8_t height, int8_t resetPin) 
  : _display(width, height, &Wire, resetPin), _width(width), _height(height) {
}

// 初始化OLED
bool OLED_Display::begin(uint8_t i2cAddress) {
  if(!_display.begin(SSD1306_SWITCHCAPVCC, i2cAddress)) {
    Serial.println("OLED init failed!");
    _initialized = false;
    return false;
  }
  
  Serial.println("OLED initialized!");
  _initialized = true;
  
  // 清除显示缓冲区
  _display.clearDisplay();
  _display.display();
  
  return true;
}

// 显示启动画面
void OLED_Display::showBootScreen() {
  if (!_initialized) return;
  
  clear();
  
  // 绘制边框 - 更靠近边缘
  drawRect(0, 0, _width, _height, SSD1306_WHITE);
  
  // 调整标题位置，确保"FOCUS CUBE"在同一行
  setTextSize(1);  // 使用1号字体确保在一行
  setTextColor(SSD1306_WHITE);
  setCursor(25, 25);
  print("FOCUS CUBE");
  
  update();
}

// 显示主界面：Focus Cube + 计时
void OLED_Display::showMainDisplay(unsigned long seconds) {
  if (!_initialized) return;
  
  clear();
  
  // 绘制边框 - 更靠近边缘
  drawRect(0, 0, _width, _height, SSD1306_WHITE);
  
  // 显示标题 - 确保在同一行
  setTextSize(1);
  setTextColor(SSD1306_WHITE);
  setCursor(25, 20);
  print("FOCUS CUBE");
  
  // 显示时间 - 使用比1号大但比2号小的字体效果
  setTextSize(2);
  setCursor(40, 35);
  
  unsigned long minutes = seconds / 60;
  unsigned long remainingSeconds = seconds % 60;
  
  // 格式化时间显示
  if (minutes < 10) print("0");
  print(String(minutes) + ":");
  if (remainingSeconds < 10) print("0");
  print(String(remainingSeconds));
  
  update();
}

// 清除显示
void OLED_Display::clear() {
  if (_initialized) {
    _display.clearDisplay();
  }
}

// 更新显示
void OLED_Display::update() {
  if (_initialized) {
    _display.display();
  }
}

// 私有工具方法
void OLED_Display::setTextSize(uint8_t size) {
  if (_initialized) {
    _display.setTextSize(size);
  }
}

void OLED_Display::setTextColor(uint16_t color) {
  if (_initialized) {
    _display.setTextColor(color);
  }
}

void OLED_Display::setCursor(int16_t x, int16_t y) {
  if (_initialized) {
    _display.setCursor(x, y);
  }
}

void OLED_Display::print(const String &text) {
  if (_initialized) {
    _display.print(text);
  }
}

void OLED_Display::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  if (_initialized) {
    _display.fillRect(x, y, w, h, color);
  }
}

void OLED_Display::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  if (_initialized) {
    _display.drawRect(x, y, w, h, color);
  }
}