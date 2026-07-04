// Ultrasonic_page.cpp
#include <Arduino.h>
#include "AppType.h"

static bool dirty = true;
static float distanceCm = 0;
static float distanceInch = 0;
static bool isActive = false;

// 使用STM32上可用的数字引脚（避免使用PA0-PA3，它们可能有特殊功能）
const int trigPin = PB0;    // Trig引脚 - 改为STM32可用的引脚
const int echoPin = PB1;    // Echo引脚 - 改为STM32可用的引脚

#define SOUND_SPEED 0.034   // 定义声音速度 in cm/uS
#define CM_TO_INCH 0.393701
long duration;

void Ultrasonic_init() {
    pinMode(trigPin, OUTPUT); // 将trigPin设置为输出
    pinMode(echoPin, INPUT);  // 将echoPin设置为Input

    // 初始状态
    digitalWrite(trigPin, LOW);
    
    isActive = true;
    dirty = true;
    
    Serial.begin(9600);     // 打开串行监视器在9600波特率，看ping结果
    Serial.println(F("Ultrasonic Sensor Started on STM32"));
}

void Ultrasonic_loop(bool ok, bool back) {
    // 处理返回按键
    if (back) {
        isActive = false;
        switchApp(APP_LIST);
        return;
    }
    
    if (isActive) {
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);

        // 将trigPin设置为HIGH状态10微秒
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        // 读取回声，以微秒为单位返回声波的传播时间
        duration = pulseIn(echoPin, HIGH);

        distanceCm = duration * SOUND_SPEED / 2;  // 计算距离 CM
        distanceInch = distanceCm * CM_TO_INCH;   // 转换为英寸

        dirty = true;

        // 在串行监视器中打印距离
        Serial.print("Distance (cm): ");
        Serial.println(distanceCm);
        // Serial.print("Distance (inch): ");
        // Serial.println(distanceInch);

        delay(100);
    }
}

// 状态获取函数
float Ultrasonic_getDistanceCm() { return distanceCm; }
float Ultrasonic_getDistanceInch() { return distanceInch; }

// 脏标记管理
bool Ultrasonic_isDirty() { return dirty; }
void Ultrasonic_clearDirty() { dirty = false; }