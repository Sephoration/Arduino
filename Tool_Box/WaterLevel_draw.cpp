// WaterLevel_draw.cpp
#include "OLED_Display.h"
#include "AppType.h"

extern int WaterLevel_getValue();
extern bool WaterLevel_isDirty();
extern void WaterLevel_clearDirty();

void WaterLevel_draw(OLED_Display& oled) {
    if (!WaterLevel_isDirty()) return;
    
    oled.clear();
    oled.showBorder();
    
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    
    // 简单显示
    oled.setCursor(20, 10);
    oled.print(F("Water Level"));
    
    oled.setCursor(40, 30);
    oled.setTextSize(2);
    
    // 修复：将int转换为String再打印
    String valueStr = String(WaterLevel_getValue());
    oled.print(valueStr);
    
    oled.update();
    WaterLevel_clearDirty();
}