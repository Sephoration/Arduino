// SmokeSensor_page.cpp
#include <Arduino.h>
#include "AppType.h"

static bool dirty = true;
static int analogValue = 0;
static bool digitalState = false;
static bool isActive = false;

// 烟雾传感器引脚定义
#define SMOKE_ANALOG_PIN PA6    // AO
#define SMOKE_DIGITAL_PIN PA7   // DO
#define LED_PIN PC13            // 使用板载LED 

// 烟雾浓度阈值
#define SMOKE_THRESHOLD 55

void SmokeSensor_init() {
    // 初始化引脚
    pinMode(SMOKE_DIGITAL_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);  // 初始LED熄灭
    
    isActive = true;
    dirty = true;
    
    Serial.println(F("Smoke Sensor Started"));
}

void SmokeSensor_loop(bool ok, bool back) {
    // 处理返回按键
    if (back) {
        // 退出前清理：确保LED熄灭
        digitalWrite(LED_PIN, HIGH);
        isActive = false;
        switchApp(APP_LIST);
        return;
    }
    
    // 读取烟雾传感器数据
    if (isActive) {
        analogValue = analogRead(SMOKE_ANALOG_PIN);    // 读取AO模拟值
        digitalState = digitalRead(SMOKE_DIGITAL_PIN); // 读取DO数字值
        
        // 控制LED：烟雾浓度高时亮起
        if (analogValue >= SMOKE_THRESHOLD) {
            digitalWrite(LED_PIN, LOW);    // 高浓度 -> LED亮
        } else {
            digitalWrite(LED_PIN, HIGH);   // 低浓度 -> LED灭
        }
        
        dirty = true;
        
        // 串口输出调试信息（可选）
        Serial.print(F("Analog: "));
        Serial.print(analogValue);
        Serial.print(F(" | Digital: "));
        Serial.println(digitalState);
    }
}

// 状态获取函数
int SmokeSensor_getAnalog() { return analogValue; }
bool SmokeSensor_getDigital() { return digitalState; }
bool SmokeSensor_isAlarm() { return analogValue >= SMOKE_THRESHOLD; }
int SmokeSensor_getThreshold() { return SMOKE_THRESHOLD; }

// 脏标记管理
bool SmokeSensor_isDirty() { return dirty; }
void SmokeSensor_clearDirty() { dirty = false; }