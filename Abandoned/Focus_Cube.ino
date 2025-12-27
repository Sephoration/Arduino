// Focus_Cube.ino - 自律立方主程序
#include "PinDefines.h"
#include "OLED_Display.h"
#include "SelfDiscipline.h"

// ==================== 全局变量定义 ====================
OLED_Display oled;

// ==================== 按键管理 ====================
// 按键状态
typedef struct {
    bool okPressed = false;
    bool backPressed = false;
    bool bothPressed = false;
    uint32_t lastDebounce = 0;
} KeyStatus;

// 优化的防抖按键读取
static void readKeys(KeyStatus &status) {
    status.okPressed = false;
    status.backPressed = false;
    status.bothPressed = false;
    
    bool nowOk = digitalRead(BUTTON_OK);
    bool nowBack = digitalRead(BUTTON_PIN);
    uint32_t now = millis();
    
    // 串口输出按键原始状态
    Serial.print(F("按键状态 - OK: "));
    Serial.print(nowOk ? F("HIGH") : F("LOW"));
    Serial.print(F(", BACK: "));
    Serial.println(nowBack ? F("HIGH") : F("LOW"));
    
    // 检测是否同时按下
    if (!nowOk && !nowBack) {
        // 100ms防抖
        if (now - status.lastDebounce < 100) return;
        status.bothPressed = true;
        status.lastDebounce = now;
        Serial.println(F("按键事件: 同时按下"));
        return;
    }
    
    // 检测下降沿和防抖
    static bool lastOk = HIGH;
    static bool lastBack = HIGH;
    static bool okPressed = false;
    static bool backPressed = false;
    static uint32_t okDebounceTime = 0;
    static uint32_t backDebounceTime = 0;
    
    // 处理OK键（确认键）
    if (lastOk != nowOk) {
        okDebounceTime = now;
    } else {
        if (now - okDebounceTime >= 100) {
            if (!lastOk && !okPressed) {
                // 按键按下（稳定）
                status.okPressed = true;
                okPressed = true;
                Serial.println(F("按键事件: 确认键按下"));
            } else if (lastOk && okPressed) {
                // 按键释放（稳定）
                okPressed = false;
                Serial.println(F("按键事件: 确认键释放"));
            }
        }
    }
    
    // 处理返回键
    if (lastBack != nowBack) {
        backDebounceTime = now;
    } else {
        if (now - backDebounceTime >= 100) {
            if (!lastBack && !backPressed) {
                // 按键按下（稳定）
                status.backPressed = true;
                backPressed = true;
                Serial.println(F("按键事件: 返回键按下"));
            } else if (lastBack && backPressed) {
                // 按键释放（稳定）
                backPressed = false;
                Serial.println(F("按键事件: 返回键释放"));
            }
        }
    }
    
    lastOk = nowOk;
    lastBack = nowBack;
}

// ==================== Arduino setup函数 ====================
void setup() {
    Serial.begin(115200);    
    
    // 配置按键引脚
    pinMode(BUTTON_OK, INPUT);
    pinMode(BUTTON_PIN, INPUT);

    // 初始化I2C引脚
    Wire.setSCL(OLED_SCL_PIN);    // 使用定义的SCL引脚
    Wire.setSDA(OLED_SDA_PIN);    // 使用定义的SDA引脚
    Wire.begin();                 // 启动I2C

    // 初始化OLED显示 初始化I2C引脚
    Wire.setSCL(OLED_SCL_PIN);    // 使用定义的SCL引脚
    Wire.setSDA(OLED_SDA_PIN);    // 使用定义的SDA引脚
    Wire.begin();                 // 启动I2C

    // 初始化OLED显示
    if (!oled.begin()) { 
        Serial.println(F("OLED初始化失败")); 
        while (1);
    }
    
    // 显示启动画面
    oled.showBootScreen();
    delay(1500);

    // 初始化自律立方
    SelfDiscipline_Init();
}

// ==================== Arduino loop函数 ====================
void loop() {
    KeyStatus keys;
    readKeys(keys);

    // 处理同时按键初始化
    if (keys.bothPressed) {
        SelfDiscipline_Reset();
    }

    // 自律立方逻辑处理
    SelfDiscipline_Loop(keys.okPressed, keys.backPressed);

    // 渲染控制 - 降低到15fps节省资源
    static uint32_t lastDraw = 0;
    if (millis() - lastDraw >= 66) {  // 15fps
        SelfDiscipline_Draw(oled);
        lastDraw = millis();
    }
}