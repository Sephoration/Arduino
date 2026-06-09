// SelfDiscipline_draw.cpp - 自律立方显示界面实现
// 实现系统的5个界面绘制，全部使用英文显示

#include "OLED_Display.h"
#include "SelfDiscipline.h"
#include "PinDefines.h"
#include <Arduino.h>

// 外部函数声明（在SelfDiscipline_page.cpp中实现）
extern SystemState SelfDiscipline_GetState();
extern UserConfig SelfDiscipline_GetConfig();
extern uint32_t SelfDiscipline_GetRemainingTime();
extern EnvironmentStatus SelfDiscipline_GetEnvStatus();
extern uint32_t SelfDiscipline_GetElapsed();
extern bool SelfDiscipline_IsDirty();
extern void SelfDiscipline_ClearDirty();

// ==================== 通用绘制工具函数 ====================
/**
 * 绘制标题（居中显示）
 */
// 通用绘制工具函数 - 修改drawTitle函数以支持左边对齐
static void drawTitle(OLED_Display& oled, const char* title) {
    oled.setTextSize(1); // 标题改为1号字体
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(5, 12); // 左边对齐，x坐标为5
    oled.print(title);
}

/**
 * 绘制时间格式（MM:SS）
 */
static void drawTime(OLED_Display& oled, int16_t x, int16_t y, uint32_t seconds, uint8_t size = 2) {
    char timeBuf[9]; // "MM:SS\0" = 6字符
    sprintf(timeBuf, "%02lu:%02lu", seconds / 60, seconds % 60);
    
    oled.setTextSize(size);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(x, y);
    oled.print(timeBuf);
}

// ==================== 主页界面绘制 ====================
/**
 * 绘制启动后的主界面
 * 显示项目名称和运行时间
 */
static void drawHomePage(OLED_Display& oled) {
    // 显示边框
    oled.showBorder();
    
    // 显示大标题（标题用2号字体）
    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);
    oled.printCentered(15, "FOCUS CUBE", 2);
    
    // 显示副标题（状态用1号字体）
    oled.setTextSize(1);
    oled.printCentered(35, "Press to start", 1);
    
    // 显示运行时间（状态用1号字体）
    uint32_t elapsed = SelfDiscipline_GetElapsed();
    char timeBuf[9];
    sprintf(timeBuf, "Run:%02lu:%02lu", elapsed / 60, elapsed % 60);
    oled.printCentered(48, timeBuf, 1);
}

// ==================== 时间选择界面绘制 ====================
/**
 * 绘制时间选择界面
 * 显示当前选择的学习时间
 */
static void drawTimeSelectPage(OLED_Display& oled) {
    // 显示边框
    oled.showBorder();
    
    UserConfig config = SelfDiscipline_GetConfig();
    
    // 显示标题（1号字体，左边对齐）
    drawTitle(oled, "SET TIME");
    
    // 显示选择的时间（改为2号字体，居中）
    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);
    char timeBuf[9];
    sprintf(timeBuf, "%02lu:%02lu", config.studyTime / 60, config.studyTime % 60);
    
    // 计算居中位置（2号字体，6字符 * 6像素 * 2倍大小 = 72像素）
    int16_t x = (OLED_WIDTH - (6 * 6 * 2)) / 2;
    oled.setCursor(x, 25);
    oled.print(timeBuf);
}

// ==================== 环境选择界面绘制 ====================
/**
 * 绘制环境检测选择界面
 * 显示当前选择的检测模式
 */
static void drawEnvSelectPage(OLED_Display& oled) {
    // 显示边框
    oled.showBorder();
    
    UserConfig config = SelfDiscipline_GetConfig();
    
    // 显示标题（1号字体，左边对齐）
    drawTitle(oled, "ENV MODE");
    
    // 显示当前模式（状态用2号字体）
    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);
    
    const char* modeText = "";
    if (config.noiseEnabled && config.lightEnabled) {
        modeText = "ALL ON";
    } else if (config.lightEnabled) {
        modeText = "LIGHT";
    } else if (config.noiseEnabled) {
        modeText = "NOISE";
    } else {
        modeText = "OFF";
    }
    
    oled.printCentered(28, modeText, 2);
    
    // 显示详细说明（状态用1号字体）
    oled.setTextSize(1);
    if (config.noiseEnabled && config.lightEnabled) {
        oled.printCentered(45, "Light+Noise", 1);
    } else if (!config.noiseEnabled && !config.lightEnabled) {
        oled.printCentered(45, "No detection", 1);
    }
}

// ==================== 倒计时界面绘制 ====================
/**
 * 绘制倒计时学习界面
 * 显示剩余时间和环境状态
 */
static void drawCountingPage(OLED_Display& oled) {
    // 显示边框
    oled.showBorder();
    
    uint32_t remaining = SelfDiscipline_GetRemainingTime();
    EnvironmentStatus status = SelfDiscipline_GetEnvStatus();
    
    // 显示倒计时（改为2号字体，居中）
    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);
    char timeBuf[9];
    sprintf(timeBuf, "%02lu:%02lu", remaining / 60, remaining % 60);
    
    // 计算居中位置（2号字体，6字符 * 6像素 * 2倍大小 = 72像素）
    int16_t x = (OLED_WIDTH - (6 * 6 * 2)) / 2;
    oled.setCursor(x, 20);
    oled.print(timeBuf);
    
    // 显示环境状态（状态用1号字体）
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    
    const char* statusText = "";
    switch (status) {
        case STATUS_GOOD:
            statusText = "Status: Learning";
            break;
        case STATUS_NO_PERSON:
            statusText = "Status: No Person";
            break;
        case STATUS_NOISE_HIGH:
            statusText = "Status: Noise High";
            break;
        case STATUS_LIGHT_DARK:
            statusText = "Status: Light Low";
            break;
        case STATUS_MULTIPLE:
            statusText = "Status: Multi Issues";
            break;
        default:
            statusText = "Status: Unknown";
            break;
    }
    
    oled.printCentered(45, statusText, 1);
    
    // 显示进度提示（状态用1号字体）
    UserConfig config = SelfDiscipline_GetConfig();
    if (config.studyTime > 0) {
        uint8_t progress = 100 - (remaining * 100 / config.studyTime);
        char progressBuf[16];
        sprintf(progressBuf, "Progress: %d%%", progress);
        oled.printCentered(55, progressBuf, 1);
    }
}

// ==================== 完成界面绘制 ====================
/**
 * 绘制学习完成界面
 */
static void drawFinishedPage(OLED_Display& oled) {
    // 显示边框
    oled.showBorder();
    
    // 显示标题（标题用2号字体）
    drawTitle(oled, "DONE!");
    
    // 显示祝贺信息（状态用2号字体）
    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);
    oled.printCentered(28, "Well Done!", 2);  // 重新计算居中位置
    
    // 显示总学习时间（状态用1号字体）
    UserConfig config = SelfDiscipline_GetConfig();
    oled.setTextSize(1);
    
    char timeBuf[20];
    sprintf(timeBuf, "Time: %d minutes", config.studyTime / 60);
    oled.printCentered(45, timeBuf, 1);
}

// ==================== 主绘制函数 ====================
/**
 * 自律立方主绘制函数
 * 根据当前状态调用对应的界面绘制函数
 */
void SelfDiscipline_Draw(OLED_Display& oled) {
    // 检查是否需要重绘
    if (!SelfDiscipline_IsDirty()) {
        return;
    }
    
    // 清空显示缓存
    oled.clear();
    
    // 根据当前状态绘制对应界面
    SystemState state = SelfDiscipline_GetState();
    
    switch (state) {
        case STATE_INIT:
            drawHomePage(oled);
            break;
            
        case STATE_SELECT_TIME:
            drawTimeSelectPage(oled);
            break;
            
        case STATE_SELECT_ENV:
            drawEnvSelectPage(oled);
            break;
            
        case STATE_COUNTING:
            drawCountingPage(oled);
            break;
            
        case STATE_FINISHED:
            drawFinishedPage(oled);
            break;
            
        default:
            // 未知状态，绘制主页
            drawHomePage(oled);
            break;
    }
    
    // 更新到显示屏
    oled.update();
    
    // 清除脏标记
    SelfDiscipline_ClearDirty();
}