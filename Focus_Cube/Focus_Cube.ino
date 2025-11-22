#include "OLED_Display.h"

// 创建OLED显示对象
OLED_Display oled;

void setup() {
  Serial.begin(115200);
  Serial.println("Focus Cube - Starting...");
  
  // 初始化OLED
  if(!oled.begin()) {
    Serial.println("OLED init failed!");
    return;
  }
  
  // 显示启动画面
  oled.showBootScreen();
  delay(1500); // 显示1.5秒启动画面
  
  Serial.println("Display initialized");
}

void loop() {
  // 显示主要界面：Focus Cube + 计时
  oled.showMainDisplay(millis() / 1000);
  delay(1000);
}