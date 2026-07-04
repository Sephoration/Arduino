// OLED_Display.cpp
#include "OLED_Display.h"
#include <Arduino.h>

// ==================== 构造函数和初始化 ====================
OLED_Display::OLED_Display()
    : _display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1) {
    // 使用PinDefines.h中的宽度和高度
}

// 初始化OLED显示
bool OLED_Display::begin() {
    // 尝试初始化SSD1306显示
    if(!_display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)){
        _initialized = false; 
        return false;
    }
    _initialized = true;
    _display.clearDisplay(); 
    _display.display();
    return true;
}

// ==================== 基础显示控制方法 ====================
void OLED_Display::clear() { 
    if(_initialized) _display.clearDisplay(); 
}

void OLED_Display::update() { 
    if(_initialized) _display.display(); 
}

void OLED_Display::showBorder() {
    if(_initialized) _display.drawRect(0, 0, OLED_WIDTH, OLED_HEIGHT, SSD1306_WHITE);
}



// ==================== 图形绘制工具方法 ====================
void OLED_Display::setTextSize(uint8_t size) { 
    if(_initialized) _display.setTextSize(size); 
}

void OLED_Display::setTextColor(uint16_t color) { 
    if(_initialized) _display.setTextColor(color); 
}

void OLED_Display::setCursor(int16_t x, int16_t y) { 
    if(_initialized) _display.setCursor(x, y); 
}

void OLED_Display::print(const String &text) { 
    if(_initialized) _display.print(text); 
}

void OLED_Display::fillRect(int16_t x,int16_t y,int16_t w,int16_t h,uint16_t color){
    if(_initialized) _display.fillRect(x,y,w,h,color);
}

void OLED_Display::drawRect(int16_t x,int16_t y,int16_t w,int16_t h,uint16_t color){
    if(_initialized) _display.drawRect(x,y,w,h,color);
}

void OLED_Display::fillScreen(uint16_t color){
    if(_initialized) _display.fillScreen(color);
}

// ==================== 新增：居中文本显示 ====================
void OLED_Display::printCentered(int16_t y, const String &text, uint8_t textSize) {
    if(!_initialized) {
        return;
    }
    
    setTextSize(textSize);
    setTextColor(SSD1306_WHITE);
    
    // 计算文本宽度（近似值：每个字符6像素 * 文字大小 * 字符数）
    uint16_t textWidth = text.length() * 6 * textSize;
    int16_t x = (OLED_WIDTH - textWidth) / 2;
    
    setCursor(x, y);
    print(text);
}





// ==================== 启动画面显示 ====================
void OLED_Display::showBootScreen(){
    if(!_initialized) {
        return;
    }
    
    clear(); 
    showBorder();
    
    setTextSize(2);
    setTextColor(SSD1306_WHITE);

    uint16_t textWidth = 3 * 6 * 2; // 3字符 * 6像素 * 2倍大小
    int16_t x = (OLED_WIDTH - textWidth) / 2;

    setCursor(x, 40);
    print("E.T.");

    update();
}