// ListPage_draw.cpp
#include "OLED_Display.h"

// 外部函数声明
extern uint8_t ListPage_getSelected(), ListPage_getCount();
extern const char* ListPage_getName(uint8_t);
extern bool ListPage_isDirty();        // 脏标记检查
extern void ListPage_clearDirty();     // 脏标记清除

// ==================== 列表页面绘制函数 ====================
// 列表页面显示渲染函数
void ListPage_draw(OLED_Display& oled) {
  // 检查脏标记，数据没变直接退出以优化性能
  if (!ListPage_isDirty()) return;
  
  // 清屏并设置基本显示
  oled.clear();
  oled.showBorder();  // 显示边框

  // 设置文本属性
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);

  // 计算垂直居中起始位置
  uint8_t totalItems = ListPage_getCount();
  uint8_t startY = (64 - totalItems * 12) / 2;  // 屏幕高度64，每项高度12

  // 遍历并绘制所有菜单项
  for (uint8_t i = 0; i < totalItems && i < 4; ++i) {
    int16_t y = startY + i * 12;  // 计算当前项Y坐标
    
    // 如果是选中项，绘制反白背景
    if (i == ListPage_getSelected()) {
      oled.fillRect(2, y - 1, 128 - 4, 10, SSD1306_WHITE);  // 反白背景
      oled.setTextColor(SSD1306_BLACK);  // 黑色文字
    } else {
      oled.setTextColor(SSD1306_WHITE);  // 白色文字
    }
    
    // 计算水平居中位置并绘制文本
    const char* itemText = ListPage_getName(i);
    uint8_t textWidth = strlen(itemText) * 6;  // 估算文本宽度(6像素/字符)
    uint8_t x = (128 - textWidth) / 2;         // 计算居中X坐标
    
    oled.setCursor(x, y);          // 设置光标位置
    oled.print(itemText);          // 绘制菜单项文本
  }
  
  // 更新显示并清除脏标记
  oled.update();
  ListPage_clearDirty();
}