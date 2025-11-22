// HomePage_page.cpp
#include <Arduino.h>
#include "AppType.h"  // 包含应用类型定义头文件

// ==================== 全局变量定义 ====================
static uint32_t bootTime = 0;   // 启动时间记录
static bool     dirty    = true;    // 脏标记 - 标识数据是否需要更新

// ==================== 初始化函数 ====================
// 主页初始化函数
void HomePage_init() { 
  bootTime = millis();  // 记录启动时间
  dirty = true;         // 设置脏标记，强制首次渲染
}

// ==================== 主循环函数 ====================
// 主页主循环处理函数
void HomePage_loop(bool ok, bool back) {
  // 处理返回按键 - 切换到列表页面
  if (back) {                       
    switchApp(APP_LIST);
    return;
  }
  
  /* 每秒自动置脏，更新运行时间 */
  static uint32_t lastSec;          // 上次记录的秒数
  uint32_t sec = (millis() - bootTime) / 1000;  // 计算当前运行秒数
  
  // 如果秒数发生变化，设置脏标记
  if (sec != lastSec) { 
    lastSec = sec; 
    dirty = true; 
  }
}

// ==================== 状态获取函数 ====================
// 获取运行时间（秒）
uint32_t HomePage_getElapsed() { 
  return (millis() - bootTime) / 1000; 
}

// ==================== 脏标记管理函数 ====================
// 检查是否需要更新显示
bool HomePage_isDirty() { 
  return dirty; 
}

// 清除脏标记
void HomePage_clearDirty() { 
  dirty = false; 
}