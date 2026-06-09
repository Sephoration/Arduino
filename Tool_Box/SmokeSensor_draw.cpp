// SmokeSensor_draw.cpp
#include "OLED_Display.h"
#include "AppType.h"

// 外部函数声明
extern int SmokeSensor_getAnalog();
extern bool SmokeSensor_getDigital();
extern bool SmokeSensor_isAlarm();
extern int SmokeSensor_getThreshold();
extern bool SmokeSensor_isDirty();
extern void SmokeSensor_clearDirty();

void SmokeSensor_draw(OLED_Display& oled) {
    if (!SmokeSensor_isDirty()) return;
    
    oled.clear();
    oled.showBorder();
    
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    
    // 显示标题
    oled.setCursor(15, 5);
    oled.print(F("Smoke Sensor"));
    
    // 绘制分隔线
    oled.drawRect(5, 15, 118, 1, SSD1306_WHITE);
    
    // 显示模拟值
    oled.setCursor(10, 25);
    oled.print(F("Analog: "));
    String analogStr = String(SmokeSensor_getAnalog());
    oled.print(analogStr);
    
    // 显示数字状态
    oled.setCursor(10, 35);
    oled.print(F("Digital: "));
    oled.print(SmokeSensor_getDigital() ? "HIGH" : "LOW");
    
    // 显示报警状态
    oled.setCursor(10, 45);
    oled.print(F("Status: "));
    if (SmokeSensor_isAlarm()) {
        oled.setTextColor(SSD1306_WHITE);
        oled.fillRect(55, 43, 40, 10, SSD1306_WHITE);
        oled.setTextColor(SSD1306_BLACK);
        oled.print(F("ALARM"));
    } else {
        oled.setTextColor(SSD1306_WHITE);
        oled.print(F("NORMAL"));
    }
    
    // 显示阈值 - 修复这里
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(10, 55);
    oled.print(F("Threshold: "));
    String thresholdStr = String(SmokeSensor_getThreshold());  // 转换为String
    oled.print(thresholdStr);
    
    oled.update();
    SmokeSensor_clearDirty();
}