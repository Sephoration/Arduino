// Photosensor_draw.cpp
#include "OLED_Display.h"
#include "AppType.h"

// 外部函数声明
extern int Photosensor_getAnalog();
extern bool Photosensor_getDigital();
extern bool Photosensor_isDirty();
extern void Photosensor_clearDirty();

// ==================== 光敏电阻绘制函数 ====================
void Photosensor_draw(OLED_Display& oled) {
    if (!Photosensor_isDirty()) return;
    
    oled.clear();
    oled.showBorder();

    // 设置文本属性
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    
    // 显示标题
    oled.setCursor(15, 5);
    oled.print(F("Photosensor"));
    
    // 绘制分隔线
    oled.drawRect(5, 15, 118, 1, SSD1306_WHITE);
    
    // 显示模拟值
    oled.setCursor(10, 25);
    oled.print(F("Analog: "));
    String analogStr = String(Photosensor_getAnalog());
    oled.print(analogStr);
    
    // 显示数字值
    oled.setCursor(10, 35);
    oled.print(F("Digital: "));
    if (Photosensor_getDigital()) {
        oled.print(F("HIGH"));
    } else {
        oled.print(F("LOW "));
    }
    
    // 显示亮度状态
    oled.setCursor(10, 45);
    oled.print(F("Status: "));
    
    int value = Photosensor_getAnalog();
    if (value > 2500) {
        oled.print(F("Very Dark"));
    } else if (value > 1500) {
        oled.print(F("Dark"));
    } else if (value > 800) {
        oled.print(F("Normal"));
    } else if (value > 300) {
        oled.print(F("Bright"));
    } else {
        oled.print(F("Very Bright"));
    }
    
    // 显示数字状态提示
    oled.setCursor(10, 55);
    if (Photosensor_getDigital()) {
        oled.print(F("DO: Dark Detected"));
    } else {
        oled.print(F("DO: Bright"));
    }
    
    oled.update();
    Photosensor_clearDirty();
}