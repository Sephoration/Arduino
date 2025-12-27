// SelfDiscipline_draw.cpp - 自律立方显示实现
#include "OLED_Display.h"
#include "SelfDiscipline.h"

// ==================== 外部函数声明 ====================
extern SystemState SelfDiscipline_GetState();
extern UserConfig SelfDiscipline_GetConfig();
extern uint32_t SelfDiscipline_GetRemainingTime();
extern EnvironmentStatus SelfDiscipline_GetEnvStatus();
extern bool SelfDiscipline_IsDirty();
extern void SelfDiscipline_ClearDirty();

// ==================== 绘制通用函数 ====================
// 绘制标题
static void drawTitle(OLED_Display& oled, const char* title) {
    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(10, 12);
    oled.print(title);
}

// 绘制按钮提示
static void drawButtonHint(OLED_Display& oled, const char* okHint, const char* backHint) {
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(10, 50);
    oled.print(okHint);
    oled.setCursor(80, 50);
    oled.print(backHint);
}

// 绘制时间格式
static void drawTime(OLED_Display& oled, int16_t x, int16_t y, uint32_t seconds) {
    char timeBuf[9];
    sprintf(timeBuf, "%02lu:%02lu", seconds / 60, seconds % 60);
    oled.setCursor(x, y);
    oled.print(timeBuf);
}

// ==================== 绘制主页界面 ====================
static void drawHomePage(OLED_Display& oled) {
    // 设置文本属性
    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);
    
    // 显示标题
    oled.setCursor(15, 12);
    oled.print(F("Focus Cube"));

    // 显示副标题
    oled.setTextSize(1);
    oled.setCursor(20, 35);
    oled.print(F("Press to start"));

    // 显示运行时间
    char buf[9];
    uint32_t sec = SelfDiscipline_GetElapsed();
    sprintf(buf, "%02lu:%02lu", sec / 60, sec % 60);
    
    oled.setCursor(49, 48);
    oled.print(buf);
}

// ==================== 绘制时间选择界面 ====================
static void drawTimeSelect(OLED_Display& oled, UserConfig config) {
    drawTitle(oled, "Time");
    
    oled.setTextSize(2); // 调整字体大小，使其更小
    oled.setTextColor(SSD1306_WHITE);
    drawTime(oled, 35, 35, config.studyTime);
    
    drawButtonHint(oled, "OK", "ADJ");
}

// ==================== 绘制环境检测选择界面 ====================
static void drawEnvSelect(OLED_Display& oled, UserConfig config) {
    drawTitle(oled, "Env");
    
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(20, 35);
    
    // 显示环境检测模式
    if (config.noiseEnabled && config.lightEnabled) {
        oled.print(F("Light+Noise"));
    } else if (!config.noiseEnabled && config.lightEnabled) {
        oled.print(F("Only Light"));
    } else if (config.noiseEnabled && !config.lightEnabled) {
        oled.print(F("Only Noise"));
    } else {
        oled.print(F("Disabled"));
    }
    
    drawButtonHint(oled, "START", "MODE");
}

// ==================== 绘制倒计时界面 ====================
static void drawCounting(OLED_Display& oled, uint32_t remaining, EnvironmentStatus status) {
    // 第一行：时间（调整为中等字体）
    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);
    drawTime(oled, 35, 20, remaining);
    
    // 第二行：状况（小号字体）
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(30, 50);
    
    switch (status) {
        case STATUS_GOOD:
            oled.print(F("状况：学习中"));
            break;
        case STATUS_NO_PERSON:
            oled.print(F("状况：无人"));
            break;
        case STATUS_NOISE_HIGH:
            oled.print(F("状况：噪音过大"));
            break;
        case STATUS_LIGHT_DARK:
            oled.print(F("状况：光线过暗"));
            break;
        case STATUS_MULTIPLE:
            oled.print(F("状况：多种问题"));
            break;
        default:
            oled.print(F("状况：未知"));
            break;
    }
}

// ==================== 绘制完成界面 ====================
static void drawFinished(OLED_Display& oled) {
    drawTitle(oled, "Done!");
    
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(20, 35);
    oled.print(F("Well Done!"));
    
    drawButtonHint(oled, "RESTART", "RESET");
}

// ==================== 主绘制函数 ====================
/**
 * @brief 自律立方主绘制函数，根据当前状态绘制相应界面
 * @param oled OLED显示屏对象引用
 * @details 仅当需要重绘时执行，根据系统状态绘制不同界面
 *          支持的界面：主页、时间选择、环境检测选择、倒计时、完成
 */
void SelfDiscipline_Draw(OLED_Display& oled) {
    // 检查是否需要重绘（脏标记）
    if (!SelfDiscipline_IsDirty()) return;
    
    // 清空显示缓存
    oled.clear();
    
    // 显示边框
    oled.showBorder();
    
    // 获取当前系统状态和配置
    SystemState state = SelfDiscipline_GetState();
    UserConfig config = SelfDiscipline_GetConfig();
    
    // 根据状态绘制不同界面
    switch (state) {
        case STATE_INIT:
            drawHomePage(oled);  // 绘制主页界面
            break;
            
        case STATE_SELECT_TIME:
            drawTimeSelect(oled, config);  // 绘制时间选择界面
            break;
        
        case STATE_SELECT_ENV:
            drawEnvSelect(oled, config);  // 绘制环境检测选择界面
            break;
        
        case STATE_COUNTING:
            // 绘制倒计时界面，传入剩余时间和环境状态
            drawCounting(oled, SelfDiscipline_GetRemainingTime(), SelfDiscipline_GetEnvStatus());
            break;
        
        case STATE_FINISHED:
            drawFinished(oled);  // 绘制完成界面
            break;
        
        default:
            drawHomePage(oled);  // 未知状态，绘制主页
            break;
    }
    
    // 更新显示内容到OLED屏幕
    oled.update();
    
    // 清除脏标记，表示已经完成重绘
    SelfDiscipline_ClearDirty();
}
