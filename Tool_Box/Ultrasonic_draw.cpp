// Ultrasonic_draw.cpp
#include "OLED_Display.h"
#include "AppType.h"

// 外部函数声明
extern float Ultrasonic_getDistanceCm();
extern float Ultrasonic_getDistanceInch();
extern bool Ultrasonic_isDirty();
extern void Ultrasonic_clearDirty();

void Ultrasonic_draw(OLED_Display& oled) {
    if (!Ultrasonic_isDirty()) return;
    
    oled.clear();
    oled.showBorder();
    
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    
    // 显示标题
    oled.setCursor(10, 5);
    oled.print(F("Ultrasonic Sensor"));
    
    // 绘制分隔线
    oled.drawRect(5, 15, 118, 1, SSD1306_WHITE);
    
    // 显示厘米距离
    oled.setCursor(10, 25);
    oled.print(F("CM: "));
    String cmStr = String(Ultrasonic_getDistanceCm(), 1);  // 保留1位小数
    oled.print(cmStr);
    oled.print(F(" cm"));
    
    // 显示英寸距离
    oled.setCursor(10, 35);
    oled.print(F("INCH: "));
    String inchStr = String(Ultrasonic_getDistanceInch(), 1);  // 保留1位小数
    oled.print(inchStr);
    oled.print(F(" inch"));
    
    // 显示距离状态
    oled.setCursor(10, 45);
    oled.print(F("Status: "));
    
    float cm = Ultrasonic_getDistanceCm();
    if (cm < 10.0) {
        oled.setTextColor(SSD1306_WHITE);
        oled.fillRect(55, 43, 40, 10, SSD1306_WHITE);
        oled.setTextColor(SSD1306_BLACK);
        oled.print(F("NEAR"));
    } else if (cm < 50.0) {
        oled.setTextColor(SSD1306_WHITE);
        oled.print(F("MID "));
    } else {
        oled.setTextColor(SSD1306_WHITE);
        oled.print(F("FAR "));
    }
    
    // 显示操作提示
    oled.setCursor(25, 55);
    oled.print(F("Press BACK to return"));
    
    oled.update();
    Ultrasonic_clearDirty();
}