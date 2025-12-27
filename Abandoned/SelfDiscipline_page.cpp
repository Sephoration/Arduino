// SelfDiscipline_page.cpp - 自律立方功能实现
#include <Arduino.h>
#include "PinDefines.h"
#include "SelfDiscipline.h"

// ==================== 环境参数阈值 ====================
// 环境参数阈值已迁移至PinDefines.h文件中

// ==================== 全局状态 ====================
static SystemState currentState = STATE_INIT;
static UserConfig userConfig;
static uint32_t remainingTime = 0;
static uint32_t startTime = 0;
static uint32_t elapsedTime = 0; // 运行时间计数器
static uint32_t appStartTime = 0; // 应用启动时间
static EnvironmentStatus envStatus = STATUS_GOOD;
static EnvironmentStatus lastEnvStatus = STATUS_GOOD;
static SystemState lastState = STATE_INIT;
static bool dirty = true;
static uint32_t lastWarningTime = 0;
static uint32_t lastCountdownUpdate = 0; // 上次倒计时更新时间
static uint32_t lastEnvCheck = 0; // 上次环境检测时间
static const uint32_t COUNTDOWN_UPDATE_INTERVAL = 1000; // 倒计时更新间隔：1秒
static const uint32_t ENV_CHECK_INTERVAL = 500; // 环境检测间隔：500毫秒
static const uint8_t SAMPLE_COUNT = 5; // 传感器采样次数
static uint16_t soundSamples[SAMPLE_COUNT] = {0}; // 噪音采样数组
static uint16_t lightSamples[SAMPLE_COUNT] = {0}; // 光照采样数组
static uint8_t sampleIndex = 0; // 采样索引

// ==================== 辅助函数 ====================
/**
 * @brief 计算数组平均值
 * @param samples 采样数组
 * @param count 采样次数
 * @return 平均值
 */
static uint16_t calculateAverage(uint16_t* samples, uint8_t count) {
    uint32_t sum = 0;
    for (uint8_t i = 0; i < count; i++) {
        sum += samples[i];
    }
    return (uint16_t)(sum / count);
}

// ==================== 蜂鸣器控制 ====================
/**
 * @brief 控制蜂鸣器发出声音
 * @param duration 蜂鸣持续时间（毫秒）
 */
void buzzerBeep(uint16_t duration) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
}

/**
 * @brief 控制蜂鸣器发出特定频率的声音
 * @param freq 蜂鸣频率（Hz）
 * @param duration 蜂鸣持续时间（毫秒）
 */
void buzzerTone(uint16_t freq, uint16_t duration) {
    // 计算周期（微秒）
    uint32_t period = 1000000 / freq;
    // 计算半周期（微秒）
    uint32_t halfPeriod = period / 2;
    // 计算循环次数
    uint32_t cycles = (duration * 1000) / period;
    
    for (uint32_t i = 0; i < cycles; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delayMicroseconds(halfPeriod);
        digitalWrite(BUZZER_PIN, LOW);
        delayMicroseconds(halfPeriod);
    }
}

/**
 * @brief 根据环境状态发出不同的蜂鸣提示
 * @param status 环境状态
 */
void playEnvBeep(EnvironmentStatus status) {
    // 不同环境状态发出不同频率和节奏的蜂鸣
    switch (status) {
        case STATUS_NO_PERSON: {
            // 无人状态：长蜂鸣
            buzzerTone(NO_PERSON_BEEP_FREQ, LONG_BEEP_DURATION);
            break;
        }
        case STATUS_NOISE_HIGH: {
            // 噪音大：短蜂鸣两次
            buzzerTone(NOISE_HIGH_BEEP_FREQ, SHORT_BEEP_DURATION);
            delay(BEEP_INTERVAL);
            buzzerTone(NOISE_HIGH_BEEP_FREQ, SHORT_BEEP_DURATION);
            break;
        }
        case STATUS_LIGHT_DARK: {
            // 光线暗：短蜂鸣三次
            buzzerTone(LIGHT_DARK_BEEP_FREQ, SHORT_BEEP_DURATION);
            delay(BEEP_INTERVAL);
            buzzerTone(LIGHT_DARK_BEEP_FREQ, SHORT_BEEP_DURATION);
            delay(BEEP_INTERVAL);
            buzzerTone(LIGHT_DARK_BEEP_FREQ, SHORT_BEEP_DURATION);
            break;
        }
        case STATUS_MULTIPLE: {
            // 多种问题：连续长蜂鸣两次
            buzzerTone(MULTIPLE_BEEP_FREQ, LONG_BEEP_DURATION);
            delay(BEEP_INTERVAL);
            buzzerTone(MULTIPLE_BEEP_FREQ, LONG_BEEP_DURATION);
            break;
        }
        default:
            break;
    }
}

/**
 * @brief 播放完成状态的蜂鸣提示
 */
void playFinishedBeep() {
    // 完成状态：欢快的节奏
    for (int i = 0; i < 3; i++) {
        buzzerTone(FINISHED_BEEP_FREQ, SHORT_BEEP_DURATION);
        delay(BEEP_INTERVAL);
        buzzerTone(FINISHED_BEEP_FREQ + 200, SHORT_BEEP_DURATION);
        delay(BEEP_INTERVAL * 2);
    }
}

// ==================== 传感器检测 ====================
/**
 * @brief 检测环境状况，包括人体存在、噪音和光照
 * @details 根据传感器检测结果综合判断环境状态，并在需要时发出警告
 *          环境状态优先级：无人 > 多种问题 > 噪音大 > 光线暗 > 环境良好
 */
void checkEnvironment() {
    // 检查是否达到环境检测间隔
    if (millis() - lastEnvCheck < ENV_CHECK_INTERVAL) {
        return;  // 未达到间隔，直接返回
    }
    lastEnvCheck = millis();  // 更新上次检测时间
    
    bool noPerson = false;    // 是否无人
    bool noiseHigh = false;   // 噪音是否过大
    bool lightDark = false;   // 光线是否过暗
    
    // 检测人体存在（PIR传感器）
    bool pirState = digitalRead(PIR_PIN);
    noPerson = !pirState;  // PIR传感器检测到人体时输出高电平
    Serial.print(F("PIR状态: "));
    Serial.println(pirState ? F("检测到人体") : F("未检测到人体"));
    
    // 检测噪音（如果开启噪音检测）
    if (userConfig.noiseEnabled) {
        // 采集噪音样本
        soundSamples[sampleIndex] = analogRead(SOUND_PIN);
        // 计算噪音平均值
        uint16_t avgSound = calculateAverage(soundSamples, SAMPLE_COUNT);
        noiseHigh = (avgSound > SOUND_THRESHOLD);
        Serial.print(F("噪音值: "));
        Serial.print(avgSound);
        Serial.print(F(", 阈值: "));
        Serial.println(SOUND_THRESHOLD);
    }
    
    // 检测光照（如果开启光照检测）
    if (userConfig.lightEnabled) {
        // 采集光照样本
        lightSamples[sampleIndex] = analogRead(LIGHT_PIN);
        // 计算光照平均值
        uint16_t avgLight = calculateAverage(lightSamples, SAMPLE_COUNT);
        lightDark = (avgLight < LIGHT_MIN);
        Serial.print(F("光照值: "));
        Serial.print(avgLight);
        Serial.print(F(", 阈值: "));
        Serial.println(LIGHT_MIN);
    }
    
    // 更新采样索引（在所有采样完成后更新）
    sampleIndex = (sampleIndex + 1) % SAMPLE_COUNT;
    
    // 综合判断环境状态，按优先级设置
    if (noPerson) {
        envStatus = STATUS_NO_PERSON;       // 无人状态优先级最高
    } else if (noiseHigh && lightDark) {
        envStatus = STATUS_MULTIPLE;        // 多种问题
    } else if (noiseHigh) {
        envStatus = STATUS_NOISE_HIGH;      // 噪音过大
    } else if (lightDark) {
        envStatus = STATUS_LIGHT_DARK;      // 光线过暗
    } else {
        envStatus = STATUS_GOOD;            // 环境良好
    }
    
    // 输出环境检测结果
    Serial.print(F("环境状态: "));
    switch (envStatus) {
        case STATUS_GOOD:
            Serial.println(F("良好"));
            break;
        case STATUS_NO_PERSON:
            Serial.println(F("无人"));
            break;
        case STATUS_NOISE_HIGH:
            Serial.println(F("噪音过大"));
            break;
        case STATUS_LIGHT_DARK:
            Serial.println(F("光线过暗"));
            break;
        case STATUS_MULTIPLE:
            Serial.println(F("多种问题"));
            break;
        default:
            Serial.println(F("未知"));
            break;
    }
    
    // 发出警告（根据不同环境状态发出不同声音，仅在倒计时状态下）
    if (envStatus != STATUS_GOOD && currentState == STATE_COUNTING) {
        if (millis() - lastWarningTime > 2000) { // 每2秒警告一次，避免频繁报警
            Serial.println(F("发出环境警告"));
            playEnvBeep(envStatus);
            lastWarningTime = millis();
        }
    }
    
    dirty = true;  // 标记状态已更新，需要重新绘制界面
}

// ==================== 倒计时处理 ====================
/**
 * @brief 更新倒计时时间
 * @details 仅在倒计时状态下更新剩余时间，当时间结束时切换到完成状态并发出提醒
 */
void updateCountdown() {
    if (currentState != STATE_COUNTING) return;  // 仅在倒计时状态下执行
    
    // 检查是否达到倒计时更新间隔
    if (millis() - lastCountdownUpdate < COUNTDOWN_UPDATE_INTERVAL) {
        return;  // 未达到间隔，直接返回
    }
    lastCountdownUpdate = millis();  // 更新上次更新时间
    
    uint32_t now = millis();
    // 计算剩余时间（秒）
    remainingTime = userConfig.studyTime - (now - startTime) / 1000;
    
    // 时间结束处理
    if (remainingTime <= 0) {
        remainingTime = 0;  // 确保剩余时间不小于0
        currentState = STATE_FINISHED;  // 切换到完成状态
        playFinishedBeep();  // 发出完成提醒（欢快的节奏）
        dirty = true;  // 标记需要重新绘制界面
    } else {
        dirty = true;  // 每秒更新一次界面
    }
}

// ==================== 获取运行时间 ====================
uint32_t SelfDiscipline_GetElapsed() {
    return elapsedTime;
}

// ==================== 初始化函数 ====================
/**
 * @brief 初始化自律立方系统
 * @details 初始化传感器引脚、系统状态、用户配置等
 *          设置默认学习时间为25分钟，默认开启所有环境检测
 */
void SelfDiscipline_Init() {
    // 初始化蜂鸣器引脚（先设置低电平，再设置为输出，避免启动时发出声音）
    digitalWrite(BUZZER_PIN, LOW);   // 先设置低电平
    pinMode(BUZZER_PIN, OUTPUT);     // 再设置为输出模式
    
    // 初始化其他传感器引脚
    pinMode(PIR_PIN, INPUT);         // 人体红外传感器输入
    pinMode(LIGHT_PIN, INPUT);       // 光照传感器输入
    pinMode(SOUND_PIN, INPUT);       // 噪音传感器输入
    
    // 初始化状态变量
    currentState = STATE_INIT;       // 初始状态为主页
    userConfig.studyTime = 25 * 60;  // 默认学习时间：25分钟
    userConfig.noiseEnabled = true;  // 默认开启噪音检测
    userConfig.lightEnabled = true;  // 默认开启光照检测
    remainingTime = 0;               // 剩余时间初始化为0
    startTime = 0;                   // 开始时间初始化为0
    appStartTime = millis();         // 记录应用启动时间
    elapsedTime = 0;                 // 运行时间初始化为0
    envStatus = STATUS_GOOD;         // 初始环境状态良好
    lastEnvStatus = STATUS_GOOD;     // 上一次环境状态
    lastState = STATE_INIT;          // 上一次系统状态
    dirty = true;                    // 标记需要重绘
    lastWarningTime = 0;             // 上一次警告时间
    
    // 串口输出：程序开启
    Serial.println(F("程序开启"));
    
    // 进入初始化状态，等待显示主页
    lastState = currentState;
}

// ==================== 重置函数 ====================
/**
 * @brief 重置自律立方系统到初始状态
 * @details 调用初始化函数重置所有状态变量
 */
void SelfDiscipline_Reset() {
    // 重置所有状态（调用初始化函数）
    SelfDiscipline_Init();
    dirty = true;  // 标记需要重绘界面
}

// ==================== 串口输出环境状态 ====================
/**
 * @brief 通过串口输出当前环境状态
 * @param status 当前环境状态
 * @details 根据环境状态枚举值，输出对应的中文描述
 */
void serialPrintEnvStatus(EnvironmentStatus status) {
    switch (status) {
        case STATUS_GOOD:
            Serial.println(F("状况：学习中"));
            break;
        case STATUS_NO_PERSON:
            Serial.println(F("状况：无人"));
            break;
        case STATUS_NOISE_HIGH:
            Serial.println(F("状况：噪音过大"));
            break;
        case STATUS_LIGHT_DARK:
            Serial.println(F("状况：光线过暗"));
            break;
        case STATUS_MULTIPLE:
            Serial.println(F("状况：多种问题"));
            break;
        default:
            break;
    }
}

// ==================== 主循环函数 ====================
/**
 * @brief 自律立方主循环函数，处理状态机逻辑
 * @param ok 确认按键是否被按下
 * @param back 返回/调整按键是否被按下
 * @details 根据当前系统状态和按键输入，处理不同状态下的逻辑
 *          状态机流程：INIT → SELECT_TIME → SELECT_ENV → COUNTING → FINISHED
 */
void SelfDiscipline_Loop(bool ok, bool back) {
    // 更新应用运行时间（秒）
    elapsedTime = (millis() - appStartTime) / 1000;
    
    // 状态机处理 - 使用清晰的状态转换逻辑
    switch (currentState) {
        case STATE_INIT: {
            // 主页状态：等待用户按下确认键开始
            if (ok) {
                // 从初始状态转换到时间选择状态
                currentState = STATE_SELECT_TIME;
                dirty = true;  // 标记需要重新绘制界面
            }
            break;
        }
        
        case STATE_SELECT_TIME: {
            // 时间选择状态
            if (ok) {
                // 确认时间选择，转换到环境检测选择状态
                currentState = STATE_SELECT_ENV;
                dirty = true;
                // 串口输出设置的时间
                Serial.print(F("设置时间为"));
                Serial.print(userConfig.studyTime / 60);
                Serial.println(F("min"));
            } else if (back) {
                // 调整时间：5分钟步进，范围5-60分钟
                userConfig.studyTime += 5 * 60;
                if (userConfig.studyTime > 60 * 60) {
                    userConfig.studyTime = 5 * 60; // 超过60分钟重置为5分钟
                }
                dirty = true;
            }
            break;
        }
        
        case STATE_SELECT_ENV: {
            // 环境检测选择状态
            if (ok) {
                // 确认环境检测选择，开始倒计时
                currentState = STATE_COUNTING;
                remainingTime = userConfig.studyTime;
                startTime = millis();  // 记录开始时间
                dirty = true;
                // 串口输出环境监测模式
                if (userConfig.noiseEnabled || userConfig.lightEnabled) {
                    Serial.print(F("开启环境监测："));
                    if (userConfig.noiseEnabled && userConfig.lightEnabled) {
                        Serial.println(F("全开"));
                    } else if (userConfig.lightEnabled) {
                        Serial.println(F("仅光线"));
                    } else {
                        Serial.println(F("仅噪音"));
                    }
                } else {
                    Serial.println(F("关闭环境监测"));
                }
            } else if (back) {
                // 切换环境检测模式：全开→仅光线→仅噪音→关闭→全开
                if (userConfig.noiseEnabled && userConfig.lightEnabled) {
                    // 切换到仅光线检测
                    userConfig.noiseEnabled = false;
                    userConfig.lightEnabled = true;
                } else if (!userConfig.noiseEnabled && userConfig.lightEnabled) {
                    // 切换到仅噪音检测
                    userConfig.noiseEnabled = true;
                    userConfig.lightEnabled = false;
                } else if (userConfig.noiseEnabled && !userConfig.lightEnabled) {
                    // 切换到关闭所有检测
                    userConfig.noiseEnabled = false;
                    userConfig.lightEnabled = false;
                } else {
                    // 切换到全开
                    userConfig.noiseEnabled = true;
                    userConfig.lightEnabled = true;
                }
                dirty = true;
            }
            break;
        }
        
        case STATE_COUNTING: {
            // 倒计时状态
            updateCountdown();  // 更新剩余时间
            checkEnvironment();  // 检测环境状况
            
            if (ok || back) {
                // 可以在这里添加暂停/继续功能
                dirty = true;
            }
            break;
        }
        
        case STATE_FINISHED: {
            // 完成状态
            if (ok) {
                // 按下确认键，重新开始（重置到初始状态）
                SelfDiscipline_Reset();
            } else if (back) {
                // 按下返回键，重新进入时间选择
                currentState = STATE_SELECT_TIME;
                dirty = true;
            }
            break;
        }
        
        default: {
            // 未知状态，重置到初始状态
            Serial.println(F("未知状态，重置系统"));
            currentState = STATE_INIT;
            dirty = true;
            break;
        }
    }
    
    // 状态变化处理
    if (currentState != lastState) {
        Serial.print(F("状态变化："));
        switch (lastState) {
            case STATE_INIT:
                Serial.print(F("初始状态 -> "));
                break;
            case STATE_SELECT_TIME:
                Serial.print(F("时间选择 -> "));
                break;
            case STATE_SELECT_ENV:
                Serial.print(F("环境选择 -> "));
                break;
            case STATE_COUNTING:
                Serial.print(F("倒计时中 -> "));
                break;
            case STATE_FINISHED:
                Serial.print(F("完成 -> "));
                break;
            default:
                Serial.print(F("未知 -> "));
                break;
        }
        switch (currentState) {
            case STATE_INIT:
                Serial.println(F("初始状态"));
                break;
            case STATE_SELECT_TIME:
                Serial.println(F("时间选择"));
                break;
            case STATE_SELECT_ENV:
                Serial.println(F("环境选择"));
                break;
            case STATE_COUNTING:
                Serial.println(F("倒计时中"));
                break;
            case STATE_FINISHED:
                Serial.println(F("完成"));
                break;
            default:
                Serial.println(F("未知"));
                break;
        }
        lastState = currentState;
    }
    
    // 环境状况变化处理
    if (envStatus != lastEnvStatus) {
        lastEnvStatus = envStatus;
        serialPrintEnvStatus(envStatus);  // 打印环境状态变化
    }
}

// ==================== 状态获取函数 ====================
SystemState SelfDiscipline_GetState() {
    return currentState;
}

UserConfig SelfDiscipline_GetConfig() {
    return userConfig;
}

uint32_t SelfDiscipline_GetRemainingTime() {
    return remainingTime;
}

EnvironmentStatus SelfDiscipline_GetEnvStatus() {
    return envStatus;
}

bool SelfDiscipline_IsDirty() {
    return dirty;
}

void SelfDiscipline_ClearDirty() {
    dirty = false;
}