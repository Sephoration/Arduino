// HomePage_draw.cpp
#include "OLED_Display.h"
#include "AppType.h"

// 外部函数声明
extern uint32_t HomePage_getElapsed();
extern bool HomePage_isDirty();
extern void HomePage_clearDirty();

// ==================== 主页绘制函数 ====================
void HomePage_draw(OLED_Display& oled) {
  if (!HomePage_isDirty()) return;
    
  oled.clear();
  oled.showBorder();

  // 设置文本属性
  oled.setTextSize(2);
  oled.setTextColor(SSD1306_WHITE);
  
  // 显示标题
  oled.setCursor(20, 12);
  oled.print(F("ToolBox"));

  // 显示副标题
  oled.setTextSize(1);
  oled.setCursor(22, 35);
  oled.print(F("Press to start"));

  // 显示运行时间
  char buf[9];
  uint32_t sec = HomePage_getElapsed();
  sprintf(buf, "%02lu:%02lu", sec / 60, sec % 60);
  
  oled.setCursor(49, 48);
  oled.print(buf);

  oled.update();
  HomePage_clearDirty();
}