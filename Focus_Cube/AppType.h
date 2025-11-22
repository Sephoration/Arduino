// AppTypes.h
#ifndef APPTYPES_H
#define APPTYPES_H

#include <stdint.h>

// 应用ID枚举 - 定义所有可用的应用
enum AppID : uint8_t { 
    APP_HOME = 0,       // 主页应用
    APP_LIST,           // 列表页面
    APP_POMODORO,       // 番茄钟应用
    APP_BREATH,         // 呼吸训练应用
    APP_WATER,          // 喝水提醒应用
    APP_COUNT           // 应用总数
};

// 声明主页应用函数
void HomePage_init();
void HomePage_loop(bool ok, bool back);
void HomePage_draw(class OLED_Display& oled);

// 声明列表页面函数
void ListPage_init();
void ListPage_loop(bool ok, bool back);
void ListPage_draw(class OLED_Display& oled);

// 声明应用切换函数
void switchApp(AppID id);

#endif