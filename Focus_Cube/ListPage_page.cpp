// ListPage_page.cpp
#include <Arduino.h>
#include <avr/pgmspace.h>
#include "AppType.h"  // 包含应用类型定义头文件

/* ---------- 状态变量 ---------- */
static uint8_t idx   = 0;           // 当前选中项的索引
static bool    dirty = true;        // 脏标记 - 标识数据是否需要更新

/* ---------- 菜单项枚举 ---------- */
enum { ITEM_HOME, ITEM_POMODORO, ITEM_BREATH, ITEM_WATER, ITEM_COUNT };

/* ---------- PROGMEM 字符串定义 ---------- */
// 将字符串存储在程序存储器中以节省RAM
const char itemHome[] PROGMEM = "Home";        // 主页项
const char itemPom[]  PROGMEM = "Pomodoro";    // 番茄钟项
const char itemBre[]  PROGMEM = "Breathing";   // 呼吸训练项
const char itemWat[]  PROGMEM = "Water";       // 喝水提醒项

// 菜单项指针数组
const char* const items[] PROGMEM = { itemHome, itemPom, itemBre, itemWat };

/* ---------- 三件套函数 ---------- */

// 列表页面初始化函数
void ListPage_init() { 
  idx = 0;        // 重置选中索引到第一项
  dirty = true;   // 设置脏标记，强制首次渲染
}

// 列表页面主循环处理函数
void ListPage_loop(bool ok, bool back) {
  // 处理返回按键 - 切换选中项
  if (back) {                           
    idx = (idx + 1) % ITEM_COUNT;  // 循环切换选中项
    dirty = true;                  // 选中变化→置脏标记
  }
  
  // 处理确认按键 - 执行对应操作
  if (ok) {                             
    if (idx == ITEM_HOME) switchApp(APP_HOME);      // 切换到主页
    // else if (idx == ITEM_POMODORO) switchApp(APP_POMODORO);
    // else if (idx == ITEM_BREATH)   switchApp(APP_BREATH);
    // else if (idx == ITEM_WATER)    switchApp(APP_WATER);
  }
}

/* ---------- 状态获取函数 ---------- */

// 获取当前选中项的索引
uint8_t ListPage_getSelected() { 
  return idx; 
}

// 获取菜单项总数
uint8_t ListPage_getCount() { 
  return ITEM_COUNT; 
}

// 获取指定索引的菜单项名称
const char* ListPage_getName(uint8_t i) {
  static char buf[18];  // 字符串缓冲区
  
  // 检查索引有效性并从PROGMEM读取字符串
  if (i < ITEM_COUNT) 
    strcpy_P(buf, (const char*)pgm_read_ptr(&items[i]));
  else 
    buf[0] = '\0';  // 无效索引返回空字符串
    
  return buf;
}

/* ---------- 脏标记管理函数 ---------- */

// 检查是否需要更新显示
bool ListPage_isDirty() { 
  return dirty; 
}

// 清除脏标记
void ListPage_clearDirty() { 
  dirty = false; 
}