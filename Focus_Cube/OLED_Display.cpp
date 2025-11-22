// OLED_Display.cpp
#include "OLED_Display.h"

// ==================== 构造函数和初始化 ====================
OLED_Display::OLED_Display(uint8_t w, uint8_t h, int8_t r)
  : _display(w, h, &Wire, r), _width(w), _height(h) {}

// 初始化OLED显示
bool OLED_Display::begin(uint8_t addr){
  // 尝试初始化SSD1306显示
  if(!_display.begin(SSD1306_SWITCHCAPVCC, addr)){
    _initialized = false; 
    return false;
  }
  _initialized = true;
  _display.clearDisplay(); 
  _display.display();
  return true;
}

// ==================== 基础显示控制方法 ====================
// 清空显示缓存
void OLED_Display::clear(){ 
  if(_initialized) _display.clearDisplay(); 
}

// 更新显示内容
void OLED_Display::update(){ 
  if(_initialized) _display.display(); 
}

// 显示边框
void OLED_Display::showBorder() {
  if(_initialized) _display.drawRect(0, 0, _width, _height, SSD1306_WHITE);
}

// ==================== 图形绘制工具方法 ====================
// 设置文字大小
void OLED_Display::setTextSize(uint8_t s){ 
  if(_initialized) _display.setTextSize(s); 
}

// 设置文字颜色
void OLED_Display::setTextColor(uint16_t c){ 
  if(_initialized) _display.setTextColor(c); 
}

// 设置光标位置
void OLED_Display::setCursor(int16_t x, int16_t y){ 
  if(_initialized) _display.setCursor(x, y); 
}

// 打印文本内容
void OLED_Display::print(const String &t){ 
  if(_initialized) _display.print(t); 
}

// 填充矩形区域
void OLED_Display::fillRect(int16_t x,int16_t y,int16_t w,int16_t h,uint16_t c){
  if(_initialized) _display.fillRect(x,y,w,h,c);
}

// 绘制矩形边框
void OLED_Display::drawRect(int16_t x,int16_t y,int16_t w,int16_t h,uint16_t c){
  if(_initialized) _display.drawRect(x,y,w,h,c);
}

// 填充整个屏幕
void OLED_Display::fillScreen(uint16_t c){
  if(_initialized) _display.fillScreen(c);
}

// ==================== 启动画面显示 ====================
// 显示启动画面
void OLED_Display::showBootScreen(){
  if(!_initialized) return;
  
  clear(); 
  drawRect(0, 0, _width, _height, SSD1306_WHITE);  // 绘制边框
  setTextSize(1); 
  setTextColor(SSD1306_WHITE);
  setCursor(25, 25); 
  print("FOCUS CUBE"); 
  update();
}