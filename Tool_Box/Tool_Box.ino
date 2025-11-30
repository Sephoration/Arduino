// Tool_Box.ino 
#include "OLED_Display.h"
#include "AppType.h"


// ==================== 硬件引脚定义 ====================
#define BUTTON_PA0 0    // 确认按键引脚
#define BUTTON_PA3 3    // 返回/切换按键引脚

// ==================== 全局变量定义 ====================
AppID currentApp = APP_HOME;
OLED_Display oled;

// 按键状态管理
struct {
    bool lastPA0 = HIGH;
    bool lastPA3 = HIGH;
    uint32_t lastDeb = 0;
} keyState;

// ==================== 优化的防抖按键读取 ====================
static void readKeys(bool &ok, bool &back) {
    ok = back = false;
    
    bool nowPA0 = digitalRead(BUTTON_PA0);
    bool nowPA3 = digitalRead(BUTTON_PA3);
    uint32_t now = millis();
    
    // 50ms防抖
    if (now - keyState.lastDeb < 50) return;
    
    // 检测下降沿
    if (keyState.lastPA0 && !nowPA0) { 
        ok = true; 
        keyState.lastDeb = now;
    }
    if (keyState.lastPA3 && !nowPA3) { 
        back = true; 
        keyState.lastDeb = now;
    }
    
    keyState.lastPA0 = nowPA0;
    keyState.lastPA3 = nowPA3;
}

// ==================== Arduino setup函数 ====================
void setup() {
    Serial.begin(115200);
    
    // 配置按键引脚
    pinMode(BUTTON_PA0, INPUT_PULLUP);
    pinMode(BUTTON_PA3, INPUT_PULLUP);

    // 初始化OLED显示
    if (!oled.begin()) { 
        Serial.println(F("OLED初始化失败")); 
        while (1);
    }
    
    // 显示启动画面
    oled.showBootScreen();
    delay(1500);

    // 初始化应用系统
    initAppRegistry();
    switchApp(APP_HOME);
}

// ==================== Arduino loop函数 ====================
void loop() {
    bool ok, back;
    readKeys(ok, back);

    // 应用逻辑处理
    if (appRouter[currentApp] && appRouter[currentApp]->loop) {
        appRouter[currentApp]->loop(ok, back);
    }

    // 渲染控制 - 降低到15fps节省资源
    static uint32_t lastDraw = 0;
    if (millis() - lastDraw >= 66) {  // 15fps
        if (appRouter[currentApp] && appRouter[currentApp]->draw) {
            appRouter[currentApp]->draw(oled);
        }
        lastDraw = millis();
    }
}

// ==================== 全局变量定义 ====================
// 应用注册表实例化
#define APP_ENTRY(id, name, init, loop, draw) {id, name, init, loop, draw},
AppInfo appRegistry[APP_COUNT] = {
    APP_REGISTRY
};
#undef APP_ENTRY

// 应用路由器初始化
AppInfo* appRouter[APP_COUNT] = {nullptr};


