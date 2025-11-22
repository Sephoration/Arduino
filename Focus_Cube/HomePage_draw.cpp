// HomePage_draw.cpp
#include "OLED_Display.h"
#include "AppType.h"  // 可选添加，保持一致性

// 外部函数声明
extern uint32_t HomePage_getElapsed();
extern bool     HomePage_isDirty();
extern void     HomePage_clearDirty();

// ==================== 主页绘制函数 ====================
// 主页显示渲染函数
void HomePage_draw(OLED_Display& oled) {
  // 检查脏标记，数据没变直接退出以优化性能
  if (!HomePage_isDirty()) return;          
    
  // 清屏并设置基本显示
  oled.clear();
  oled.showBorder();  // 显示边框

  // 设置文本属性
  oled.setTextSize(2);        // 使用大号字体显示标题
  oled.setTextColor(SSD1306_WHITE);
  
  // 居中显示大标题 "FOCUS CUBE"
  // "FOCUS CUBE" 大字体约 11字符 × 12像素/字符 = 132像素，需要居中调整
  oled.setCursor(5, 12);      // 调整到更靠左的位置使其居中
  oled.print(F("FOCUS CUBE"));

  // 设置小号字体显示副标题
  oled.setTextSize(1);
  // "Press to start" 约14字符 × 6像素/字符 = 84像素
  // (128 - 84) / 2 = 22，所以设置X坐标为22使其居中
  oled.setCursor(22, 35);     // 修正居中显示提示文字
  oled.print(F("Press to start"));

  // 格式化并显示运行时间
  char buf[9];  // 时间字符串缓冲区 "MM:SS\0"
  uint32_t sec = HomePage_getElapsed();  // 获取运行秒数
  sprintf(buf, "%02lu:%02lu", sec / 60, sec % 60);  // 格式化为 "分:秒"
  
  // 居中显示运行时间
  // "00:00" 约5字符 × 6像素/字符 = 30像素
  // (128 - 30) / 2 = 49，所以设置X坐标为49使其居中
  oled.setCursor(49, 48);     // 修正居中显示运行时间
  oled.print(buf);

  // 更新显示并清除脏标记
  oled.update();
  HomePage_clearDirty();      // 画完清脏标记
}