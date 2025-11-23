// HomePage_page.cpp
#include <Arduino.h>
#include "AppType.h"

// ==================== 全局变量定义 ====================
static uint32_t bootTime = 0;
static bool dirty = true;

// ==================== 初始化函数 ====================
void HomePage_init() { 
  bootTime = millis();
  dirty = true;
}

// ==================== 主循环函数 ====================
void HomePage_loop(bool ok, bool back) {
  if (back) {
    switchApp(APP_LIST);
    return;
  }
  
  // 每秒自动置脏，更新运行时间
  static uint32_t lastSec = 0;
  uint32_t sec = (millis() - bootTime) / 1000;
  
  if (sec != lastSec) { 
    lastSec = sec; 
    dirty = true; 
  }
}

// ==================== 状态获取函数 ====================
uint32_t HomePage_getElapsed() { 
  return (millis() - bootTime) / 1000; 
}

bool HomePage_isDirty() { 
  return dirty; 
}

void HomePage_clearDirty() { 
  dirty = false; 
}