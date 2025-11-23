// Joystick_draw.cpp
#include "OLED_Display.h"
#include "AppType.h"

// 外部函数声明
extern int Joystick_getX();
extern int Joystick_getY(); 
extern int Joystick_getZ();
extern bool Joystick_isDirty();
extern void Joystick_clearDirty();

void Joystick_draw(OLED_Display& oled) {
    if (!Joystick_isDirty()) return;
    
    oled.clear();
    oled.showBorder();
    
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    
    // 显示标题
    oled.setCursor(15, 5);
    oled.print(F("Joystick Sensor"));
    
    // 绘制分隔线
    oled.drawRect(5, 15, 118, 1, SSD1306_WHITE);
    
    // 显示X轴数值
    oled.setCursor(10, 25);
    oled.print(F("X: "));
    String xStr = String(Joystick_getX());
    oled.print(xStr);
    
    // 显示Y轴数值  
    oled.setCursor(10, 35);
    oled.print(F("Y: "));
    String yStr = String(Joystick_getY());
    oled.print(yStr);
    
    // 显示Z轴状态
    oled.setCursor(10, 45);
    oled.print(F("Z: "));
    if (Joystick_getZ() == HIGH) {
        oled.print(F("Released"));
    } else {
        oled.print(F("Pressed "));
    }
    
    // 显示操作提示
    oled.setCursor(25, 55);
    oled.print(F("Press BACK to return"));
    
    oled.update();
    Joystick_clearDirty();
}