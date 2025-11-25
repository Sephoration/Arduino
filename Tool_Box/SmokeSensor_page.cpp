// SmokeSensor_page.cpp
#include <Arduino.h>
#include "AppType.h"

static bool dirty = true;
static int analogValue = 0;
static int filteredValue = 0;
static bool digitalState = false;
static bool isActive = false;
static unsigned long lastReadTime = 0;

// 烟雾传感器引脚定义
#define SMOKE_ANALOG_PIN PA6    // AO
#define SMOKE_DIGITAL_PIN PA7   // DO
#define LED_PIN PC13            // 使用板载LED 

// 烟雾浓度阈值和配置
#define SMOKE_THRESHOLD 55
#define READ_INTERVAL 200       // 读取间隔(ms)
#define FILTER_ALPHA 0.3        // 滤波系数 (0-1, 越小滤波越强)
#define DEBOUNCE_TIME 1000      // 报警消抖时间(ms)
static unsigned long alarmStartTime = 0;
static bool stableAlarmState = false;

void SmokeSensor_init() {
    // 初始化引脚
    pinMode(SMOKE_DIGITAL_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);  // 初始LED熄灭
    
    // 初始化变量
    analogValue = 0;
    filteredValue = 0;
    digitalState = false;
    isActive = true;
    dirty = true;
    lastReadTime = 0;
    alarmStartTime = 0;
    stableAlarmState = false;
    
    // 预读几次传感器以稳定数值
    for(int i = 0; i < 5; i++) {
        analogRead(SMOKE_ANALOG_PIN);
        delay(10);
    }
    
    Serial.println(F("Smoke Sensor Started - Optimized Version"));
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
    
    // 限速读取：避免读取过于频繁
    unsigned long currentTime = millis();
    if (isActive && (currentTime - lastReadTime >= READ_INTERVAL)) {
        lastReadTime = currentTime;
        
        // 读取原始传感器数据
        int rawValue = analogRead(SMOKE_ANALOG_PIN);
        bool rawDigital = digitalRead(SMOKE_DIGITAL_PIN);
        
        // 应用低通滤波平滑数据
        filteredValue = (int)(FILTER_ALPHA * rawValue + (1 - FILTER_ALPHA) * filteredValue);
        analogValue = filteredValue;
        digitalState = rawDigital;
        
        // 报警状态消抖处理
        bool currentAlarm = (analogValue >= SMOKE_THRESHOLD);
        if (currentAlarm != stableAlarmState) {
            if (alarmStartTime == 0) {
                alarmStartTime = currentTime;
            } else if (currentTime - alarmStartTime >= DEBOUNCE_TIME) {
                stableAlarmState = currentAlarm;
                alarmStartTime = 0;
            }
        } else {
            alarmStartTime = 0; // 重置计时器
        }
        
        // 控制LED：使用稳定后的报警状态
        digitalWrite(LED_PIN, stableAlarmState ? LOW : HIGH);
        
        dirty = true;
        
        // 串口输出调试信息
        if (dirty) { // 只在数据变化时输出
            Serial.print(F("Raw: "));
            Serial.print(rawValue);
            Serial.print(F(" | Filtered: "));
            Serial.print(filteredValue);
            Serial.print(F(" | Digital: "));
            Serial.print(digitalState);
            Serial.print(F(" | Alarm: "));
            Serial.println(stableAlarmState ? "YES" : "NO");
        }
    }
}

// 新增函数：强制重置传感器状态
void SmokeSensor_reset() {
    filteredValue = analogRead(SMOKE_ANALOG_PIN); // 重新校准基准值
    stableAlarmState = false;
    alarmStartTime = 0;
    digitalWrite(LED_PIN, HIGH);
    dirty = true;
    
    Serial.println(F("Smoke Sensor Reset"));
}

// 新增函数：手动校准环境基准
void SmokeSensor_calibrate() {
    int sum = 0;
    int samples = 10;
    
    for(int i = 0; i < samples; i++) {
        sum += analogRead(SMOKE_ANALOG_PIN);
        delay(50);
    }
    
    filteredValue = sum / samples;
    analogValue = filteredValue;
    dirty = true;
    
    Serial.print(F("Calibrated baseline: "));
    Serial.println(filteredValue);
}

// 状态获取函数
int SmokeSensor_getAnalog() { return analogValue; }
int SmokeSensor_getFilteredAnalog() { return filteredValue; } // 新增：获取滤波后的值
bool SmokeSensor_getDigital() { return digitalState; }
bool SmokeSensor_isAlarm() { return stableAlarmState; } // 使用稳定报警状态
int SmokeSensor_getThreshold() { return SMOKE_THRESHOLD; }

// 脏标记管理
bool SmokeSensor_isDirty() { return dirty; }
void SmokeSensor_clearDirty() { dirty = false; }