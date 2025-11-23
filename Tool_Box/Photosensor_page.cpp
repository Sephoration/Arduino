// Photosensor_page.cpp
#include <Arduino.h>
#include "AppType.h"

// ==================== 全局变量定义 ====================
static bool dirty = true;
static int analogValue = 0;
static bool digitalState = false;
static bool isActive = false;

// 光敏电阻引脚定义 - 四引脚模块
#define PHOTO_ANALOG_PIN PA5    // AO - 模拟输出
#define PHOTO_DIGITAL_PIN PA6   // DO - 数字输出

// ==================== 初始化函数 ====================
void Photosensor_init() {
    // 初始化引脚
    pinMode(PHOTO_ANALOG_PIN, INPUT);   // 模拟输入
    pinMode(PHOTO_DIGITAL_PIN, INPUT);  // 数字输入
    
    isActive = true;
    dirty = true;
    
    Serial.begin(9600);
    Serial.println(F("Photosensor Started (4-pin)"));
}

// ==================== 主循环函数 ====================
void Photosensor_loop(bool ok, bool back) {
    // 处理返回按键
    if (back) {
        isActive = false;
        switchApp(APP_LIST);
        return;
    }
    
    // 读取光敏电阻数据
    if (isActive) {
        // 读取模拟值和数字值
        analogValue = analogRead(PHOTO_ANALOG_PIN);     // AO引脚
        digitalState = digitalRead(PHOTO_DIGITAL_PIN);  // DO引脚
        
        dirty = true;
        
        // 串口输出调试信息
        Serial.print(F("Light - Analog: "));
        Serial.print(analogValue);
        Serial.print(F(" | Digital: "));
        Serial.println(digitalState ? "HIGH" : "LOW");
        
        delay(100);
    }
}

// ==================== 状态获取函数 ====================
int Photosensor_getAnalog() { 
    return analogValue; 
}

bool Photosensor_getDigital() { 
    return digitalState; 
}

bool Photosensor_isDirty() { 
    return dirty; 
}

void Photosensor_clearDirty() { 
    dirty = false; 
}