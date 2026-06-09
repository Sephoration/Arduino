// 应用类型定义和自动注册

#ifndef APPTYPES_H         // 防止重复包含
#define APPTYPES_H         // 定义宏，防止重复包含
#include <stdint.h>
#include <avr/pgmspace.h>
#include "OLED_Display.h"

// ============================================================================
// 1. 应用ID枚举 - 添加新应用的ID
// ============================================================================
enum AppID : uint8_t { 
    APP_HOME = 0,       // 主页应用
    APP_LIST,           // 列表页面
    APP_COUNT           // 统计总数，无实际应用，仅用于数组边界
};

// ============================================================================
// 2. 函数声明区域 - 声明新应用的三个函数
// ============================================================================
// 主页应用函数
void HomePage_init();
void HomePage_loop(bool ok, bool back);
void HomePage_draw(OLED_Display& oled);

// 列表页面函数  
void ListPage_init();
void ListPage_loop(bool ok, bool back);
void ListPage_draw(OLED_Display& oled);

// 声明新应用的函数，格式如下
// void YourApp_init();
// void YourApp_loop(bool ok, bool back);
// void YourApp_draw(OLED_Display& oled);

// ============================================================================
// 3. 应用注册表 - 注册新应用
// ============================================================================
#define APP_REGISTRY \
    APP_ENTRY(APP_HOME, F("Home"), HomePage_init, HomePage_loop, HomePage_draw) \
    APP_ENTRY(APP_LIST, F("List"), ListPage_init, ListPage_loop, ListPage_draw) \
    // ！！！在这里注册新应用 ！！！
    // 格式：APP_ENTRY(APP_你的功能名, F("显示名称"), 初始化函数, 循环函数, 绘制函数)

// ============================================================================
// 核心框架 - 以下代码不需要修改
// ============================================================================
// 应用函数指针类型定义
typedef void (*AppInitFunc)();
typedef void (*AppLoopFunc)(bool ok, bool back);
typedef void (*AppDrawFunc)(OLED_Display&);

// 应用信息结构体
struct AppInfo { 
    AppID id;
    const __FlashStringHelper* name;    // 使用FlashString节省RAM
    AppInitFunc init;
    AppLoopFunc loop;
    AppDrawFunc draw;
};

// 全局变量声明
extern AppID currentApp;
extern AppInfo appRegistry[APP_COUNT];
extern AppInfo* appRouter[APP_COUNT];

// 初始化应用注册表
inline void initAppRegistry() {
    for (uint8_t i = 0; i < APP_COUNT; i++) {
        appRouter[appRegistry[i].id] = &appRegistry[i];
    }
}

// 切换到指定应用
inline void switchApp(AppID id) {
    if (id < APP_COUNT && appRouter[id] && appRouter[id]->init) {
        currentApp = id;
        appRouter[id]->init();
    }
}

// 获取应用名称（安全版本）
inline const char* getAppName(AppID id) {
    static char buffer[16];
    if (id < APP_COUNT && appRouter[id]) {
        strcpy_P(buffer, (const char*)appRouter[id]->name);
        return buffer;
    }
    return "";
}

#endif