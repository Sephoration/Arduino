// ListPage_page.cpp - 修复版
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "AppType.h"

/* ---------- 状态变量 ---------- */
static uint8_t selectedIndex = 0;
static bool dirty = true;

/* ---------- 自动菜单项管理 ---------- */
// 计算有效菜单项数量（排除列表页面自身）
static uint8_t getMenuItemCount() {
    uint8_t count = 0;
    for (uint8_t i = 0; i < APP_COUNT; i++) {
        if (i != APP_LIST) count++;
    }
    return count;
}

// 通过菜单索引获取实际应用ID
static AppID getAppIDFromMenuIndex(uint8_t menuIndex) {
    uint8_t realCount = 0;
    for (uint8_t appId = 0; appId < APP_COUNT; appId++) {
        if (appId != APP_LIST) {
            if (realCount == menuIndex) {
                return (AppID)appId;
            }
            realCount++;
        }
    }
    return APP_HOME;
}

/* ---------- 三件套函数 ---------- */
void ListPage_init() { 
    selectedIndex = 0;
    dirty = true;
}

void ListPage_loop(bool ok, bool back) {
    uint8_t itemCount = getMenuItemCount();
    
    if (back) {
        selectedIndex = (selectedIndex + 1) % itemCount;
        dirty = true;
        return;
    }
    
    if (ok && selectedIndex < itemCount) {
        AppID targetApp = getAppIDFromMenuIndex(selectedIndex);
        switchApp(targetApp);
    }
}

/* ---------- 状态获取函数 ---------- */
uint8_t ListPage_getSelected() { 
    return selectedIndex; 
}

uint8_t ListPage_getCount() { 
    return getMenuItemCount(); 
}

const char* ListPage_getName(uint8_t index) {
    if (index < getMenuItemCount()) {
        AppID appId = getAppIDFromMenuIndex(index);
        return getAppName(appId);
    }
    return "";
}

/* ---------- 脏标记管理 ---------- */
bool ListPage_isDirty() { return dirty; }
void ListPage_clearDirty() { dirty = false; }