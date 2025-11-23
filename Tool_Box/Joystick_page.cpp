// Joystick_page.cpp
#include <Arduino.h>
#include "AppType.h"

static bool dirty = true;
static int xValue = 0;
static int yValue = 0;
static int zValue = 0;
static bool isActive = false;

// 摇杆传感器引脚定义
#define JOYSTICK_X_PIN PA5    // X轴模拟输入
#define JOYSTICK_Y_PIN PA6    // Y轴模拟输入  
#define JOYSTICK_Z_PIN PA7    // Z轴数字按键（改为PA5，避免冲突）

void Joystick_init() {
    // 初始化引脚
    pinMode(JOYSTICK_Z_PIN, INPUT);
    
    isActive = true;
    dirty = true;
    
    Serial.println(F("Joystick Sensor Started"));
}

void Joystick_loop(bool ok, bool back) {
    // 处理返回按键
    if (back) {
        isActive = false;
        switchApp(APP_LIST);
        return;
    }
    
    // 读取摇杆传感器数据
    if (isActive) {
        xValue = analogRead(JOYSTICK_X_PIN);  // X轴
        yValue = analogRead(JOYSTICK_Y_PIN);  // Y轴
        zValue = digitalRead(JOYSTICK_Z_PIN); // Z轴按键
        
        dirty = true;
        
        // 串口输出调试信息（可选）
        Serial.print(F("X:"));
        Serial.print(xValue);
        Serial.print(F(" | Y:"));
        Serial.print(yValue); 
        Serial.print(F(" | Z:"));
        Serial.println(zValue);
    }
}

// 状态获取函数
int Joystick_getX() { return xValue; }
int Joystick_getY() { return yValue; }
int Joystick_getZ() { return zValue; }

// 脏标记管理
bool Joystick_isDirty() { return dirty; }
void Joystick_clearDirty() { dirty = false; }