// 硬件初始化（setup）
// 按键扫描和防抖
// 主循环调度
// 调用 SelfDiscipline_Loop 和 SelfDiscipline_Draw
// 双键同时按的全局处理

// Focus_Cube.ino - 自律立方主程序
// 硬件初始化、按键处理、主循环调度

#include "PinDefines.h"
#include "OLED_Display.h"
#include "SelfDiscipline.h"

// ==================== 全局对象定义 ====================
OLED_Display oled;        // OLED显示对象

// ==================== 按键状态结构 ====================
typedef struct {
    bool okPressed = false;     // 确认键按下
    bool backPressed = false;   // 返回键按下
    bool bothPressed = false;   // 双键同时按下
} KeyStatus;

// ==================== 按键读取函数 ====================
/**
 * 读取按键状态，带防抖处理
 * 检测单键按下和双键同时按下
 */
static void readKeys(KeyStatus &status) {
    // 简单直接的读取
    bool okNow = (digitalRead(BUTTON_OK) == LOW);
    bool backNow = (digitalRead(BUTTON_PIN) == LOW);
    
    static bool okLast = false;
    static bool backLast = false;
    static uint32_t lastReadTime = 0;
    
    uint32_t now = millis();
    
    // 简单的去抖动
    if (now - lastReadTime < 50) {
        return;  // 跳过，避免过于频繁
    }
    lastReadTime = now;
    
    // 边缘检测
    status.okPressed = (okNow && !okLast);
    status.backPressed = (backNow && !backLast);
    status.bothPressed = (okNow && backNow);
    
    // 保存状态供下次比较
    okLast = okNow;
    backLast = backNow;
    
    #ifdef DEBUG_KEYS
    if (status.okPressed) Serial.println("Key: OK pressed");
    if (status.backPressed) Serial.println("Key: BACK pressed");
    if (status.bothPressed) Serial.println("Key: BOTH pressed");
    #endif
}

// ==================== Arduino Setup函数 ====================
void setup() {
    // 初始化串口调试（115200波特率）
    Serial.begin(115200);
    
    // 配置按键引脚（使用内置上拉电阻）
    pinMode(BUTTON_OK, INPUT_PULLUP);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    
    // 对于STM32，可以直接开始Wire
    Wire.begin();
    
    // 初始化OLED显示
    if (!oled.begin()) {
        // OLED初始化失败，蜂鸣器报警并阻塞
        pinMode(BUZZER_PIN, OUTPUT);
        while (1) {
            digitalWrite(BUZZER_PIN, LOW);   // 响
            delay(200);
            digitalWrite(BUZZER_PIN, HIGH);  // 停
            delay(800);
        }
    }
    
    // 显示启动画面
    oled.showBootScreen();
    delay(1500);  // 显示1.5秒启动画面
    
    // 初始化自律立方系统
    SelfDiscipline_Init();
}

// ==================== Arduino Loop函数 ====================
void loop() {
    // 1. 读取按键状态
    KeyStatus keys;
    readKeys(keys);
    
    // 2. 处理双键同时按下（系统重置）
    if (keys.bothPressed) {
        SelfDiscipline_Reset();
        
        // 重置反馈：短蜂鸣一声（低电平触发）
        digitalWrite(BUZZER_PIN, LOW);   // 响
        delay(100);
        digitalWrite(BUZZER_PIN, HIGH);  // 停
        
        delay(300);
        return;
    }
    
    // 3. 处理自律立方逻辑（传递单键事件）
    SelfDiscipline_Loop(keys.okPressed, keys.backPressed);
    
    // 4. 控制显示刷新率（约15fps = 66ms/帧）
    static uint32_t lastDrawTime = 0;
    if (millis() - lastDrawTime >= 66) {
        SelfDiscipline_Draw(oled);
        lastDrawTime = millis();
    }
    
    // 5. 小型延迟，避免过于频繁的循环（节省CPU）
    delay(10);
}