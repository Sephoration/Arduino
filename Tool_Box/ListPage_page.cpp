// ListPage_page.cpp - 页面切换版
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "AppType.h"

/* ---------- 状态变量 ---------- */
static uint8_t selectedIndex = 0;
static uint8_t currentPage = 0;      // 当前页面索引
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

// 获取当前页面显示的项目数量（最多4个）
static uint8_t getCurrentPageItemCount() {
    uint8_t totalItems = getMenuItemCount();
    uint8_t startIndex = currentPage * 4;
    
    if (startIndex >= totalItems) return 0;
    return min(4, totalItems - startIndex);
}

// 获取当前页面起始索引
static uint8_t getPageStartIndex() {
    return currentPage * 4;
}

// 获取总页数
static uint8_t getTotalPages() {
    uint8_t totalItems = getMenuItemCount();
    return (totalItems + 3) / 4;  // 向上取整
}

/* ---------- 三件套函数 ---------- */
void ListPage_init() { 
    selectedIndex = 0;
    currentPage = 0;
    dirty = true;
}

void ListPage_loop(bool ok, bool back) {
    uint8_t totalItems = getMenuItemCount();
    uint8_t currentPageItems = getCurrentPageItemCount();
    uint8_t totalPages = getTotalPages();
    
    if (back) {
        // 向后选择
        selectedIndex++;
        
        // 如果超过当前页面项目数，切换到下一页
        if (selectedIndex >= currentPageItems) {
            if (currentPage < totalPages - 1) {
                // 有下一页，切换到下一页
                currentPage++;
                selectedIndex = 0;
            } else {
                // 已经是最后一页，回到第一页
                currentPage = 0;
                selectedIndex = 0;
            }
        }
        
        dirty = true;
        return;
    }
    
    if (ok && selectedIndex < currentPageItems) {
        uint8_t actualIndex = getPageStartIndex() + selectedIndex;
        if (actualIndex < totalItems) {
            AppID targetApp = getAppIDFromMenuIndex(actualIndex);
            switchApp(targetApp);
        }
    }
}

/* ---------- 状态获取函数 ---------- */
uint8_t ListPage_getSelected() { 
    return selectedIndex; 
}

uint8_t ListPage_getCurrentPage() {
    return currentPage;
}

uint8_t ListPage_getPageItemCount() {
    return getCurrentPageItemCount();
}

uint8_t ListPage_getTotalPages() {
    return getTotalPages();
}

const char* ListPage_getName(uint8_t index) {
    uint8_t actualIndex = getPageStartIndex() + index;
    if (actualIndex < getMenuItemCount()) {
        AppID appId = getAppIDFromMenuIndex(actualIndex);
        return getAppName(appId);
    }
    return "";
}

/* ---------- 脏标记管理 ---------- */
bool ListPage_isDirty() { return dirty; }
void ListPage_clearDirty() { dirty = false; }