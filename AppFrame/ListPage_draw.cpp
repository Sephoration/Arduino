// ListPage_draw.cpp
#include "OLED_Display.h"
#include "AppType.h"

// 外部函数声明
extern uint8_t ListPage_getSelected();
extern uint8_t ListPage_getCount();
extern const char* ListPage_getName(uint8_t);
extern bool ListPage_isDirty();
extern void ListPage_clearDirty();

// ==================== 列表页面绘制函数 ====================
void ListPage_draw(OLED_Display& oled) {
  if (!ListPage_isDirty()) return;
  
  oled.clear();
  oled.showBorder();

  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);

  uint8_t totalItems = ListPage_getCount();
  uint8_t startY = (64 - totalItems * 12) / 2;

  for (uint8_t i = 0; i < totalItems && i < 4; ++i) {
    int16_t y = startY + i * 12;
    
    if (i == ListPage_getSelected()) {
      oled.fillRect(2, y - 1, 128 - 4, 10, SSD1306_WHITE);
      oled.setTextColor(SSD1306_BLACK);
    } else {
      oled.setTextColor(SSD1306_WHITE);
    }
    
    const char* itemText = ListPage_getName(i);
    uint8_t textWidth = strlen(itemText) * 6;
    uint8_t x = (128 - textWidth) / 2;
    
    oled.setCursor(x, y);
    oled.print(itemText);
  }
  
  oled.update();
  ListPage_clearDirty();
}