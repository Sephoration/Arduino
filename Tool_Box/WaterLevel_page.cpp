// WaterLevel_page.cpp
#include <Arduino.h>
#include "AppType.h"

static bool dirty = true;
static int waterLevel = 0;
static bool isActive = false;

// STM32F103C6引脚定义
#define WATER_SENSOR_PIN PA7    // 水位传感器连接到PA7
#define STATUS_LED_PIN PC13     // 板载LED

void WaterLevel_init() {
    // 初始化硬件
    pinMode(STATUS_LED_PIN, OUTPUT);
    digitalWrite(STATUS_LED_PIN, HIGH);  // 初始状态：LED熄灭
    
    isActive = true;  // 标记应用为激活状态
    dirty = true;     // 强制首次渲染
    
    Serial.println(F("Water Level Monitor Started"));
}

void WaterLevel_loop(bool ok, bool back) {
    // 处理返回按键 - 退出到列表页面
    if (back) {
        // 退出前清理：确保LED熄灭
        digitalWrite(STATUS_LED_PIN, HIGH);
        isActive = false;
        switchApp(APP_LIST);
        return;
    }
    
    // 读取水位传感器
    waterLevel = analogRead(WATER_SENSOR_PIN);
    
    // 只有在本应用激活时才控制LED
    if (isActive) {
        // 控制LED：高水位时亮起，低水位时熄灭
        if(waterLevel > 700) {
            digitalWrite(STATUS_LED_PIN, LOW);   // 高水位 -> LED亮
        } else {
            digitalWrite(STATUS_LED_PIN, HIGH);  // 低水位 -> LED灭
        }
    }
    
    // 设置脏标记触发界面更新
    dirty = true;
    
    // 可选：串口输出调试信息
    // Serial.println(waterLevel);
}

// 状态获取函数
int WaterLevel_getValue() { 
    return waterLevel; 
}

// 脏标记管理函数
bool WaterLevel_isDirty() { 
    return dirty; 
}

void WaterLevel_clearDirty() { 
    dirty = false; 
}