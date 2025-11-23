// ListPage_draw.cpp
#include "OLED_Display.h"
#include "AppType.h"

// 外部函数声明
extern uint8_t ListPage_getSelected();
extern uint8_t ListPage_getCurrentPage();
extern uint8_t ListPage_getPageItemCount();
extern uint8_t ListPage_getTotalPages();
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

  uint8_t selectedIndex = ListPage_getSelected();
  uint8_t currentPage = ListPage_getCurrentPage();
  uint8_t totalPages = ListPage_getTotalPages();
  uint8_t pageItemCount = ListPage_getPageItemCount();
  
  // 计算垂直起始位置
  uint8_t startY = (64 - pageItemCount * 12) / 2;

  // 显示页面信息
  oled.setCursor(2, 2);
  
  // 使用String来构建页面信息
  String pageInfo = String(currentPage + 1) + "/" + String(totalPages);
  oled.print(pageInfo);

  // 显示菜单项
  for (uint8_t i = 0; i < pageItemCount; ++i) {
    int16_t y = startY + i * 12;
    
    if (i == selectedIndex) {
      oled.fillRect(2, y - 1, 124, 10, SSD1306_WHITE);
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
  
  // 显示操作提示
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(20, 55);
  
  // 使用String构建提示信息
  String hintText;
  if (totalPages > 1) {
    hintText = "Page " + String(currentPage + 1) + "/" + String(totalPages);
  } else {
    hintText = "Press BACK to switch";
  }
  oled.print(hintText);
  
  oled.update();
  ListPage_clearDirty();
}