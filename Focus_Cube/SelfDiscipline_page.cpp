// SelfDiscipline_page.cpp - 自律立方核心逻辑实现
// 实现系统的5个状态处理、倒计时、环境检测和蜂鸣器控制

#include <Arduino.h>
#include "PinDefines.h"
#include "SelfDiscipline.h"

// ==================== 辅助函数：时间单位转换 ====================
static uint16_t minutesToSeconds(uint16_t minutes) {
    return minutes * 60;
}

static uint16_t secondsToMinutes(uint16_t seconds) {
    return seconds / 60;
}

// ==================== 全局状态变量 ====================
// 注意：这些变量只在当前文件内可见，通过函数接口对外提供访问

// 全局状态变量
static SystemState currentState = STATE_INIT;      // 当前系统状态
static UserConfig userConfig;                      // 用户配置
static uint32_t remainingTime = 0;                 // 剩余时间（秒）
static uint32_t startTime = 0;                     // 倒计时开始时间
static uint32_t appStartTime = 0;                  // 应用启动时间
static EnvironmentStatus envStatus = STATUS_GOOD;  // 当前环境状态
static EnvironmentStatus lastEnvStatus = STATUS_GOOD; // 上次环境状态
static bool dirty = true;                          // 脏标记，是否需要重绘
static uint32_t lastWarningTime = 0;               // 上次警告时间
static bool hasPirSensor = true;                   // 是否安装了PIR传感器 - 设置为true启用人体检测

// 倒计时和环境检测的定时器
static uint32_t lastCountdownUpdate = 0;           // 上次倒计时更新时间
static uint32_t lastEnvCheck = 0;                  // 上次环境检测时间

// 传感器采样缓冲（用于平均值滤波）
static const uint8_t SAMPLE_COUNT = 5;             // 采样次数
static uint16_t soundSamples[SAMPLE_COUNT] = {0};  // 噪音采样数组
static uint16_t lightSamples[SAMPLE_COUNT] = {0};  // 光照采样数组
static uint8_t sampleIndex = 0;                    // 采样索引

// ==================== 辅助函数声明 ====================
static uint16_t calculateAverage(uint16_t* samples, uint8_t count);
static EnvironmentStatus checkEnvironment();
static void updateCountdown();
static void handleInitState(bool ok, bool back);
static void handleTimeSelectState(bool ok, bool back);
static void handleEnvSelectState(bool ok, bool back);
static void handleCountingState(bool ok, bool back);
static void handleFinishedState(bool ok, bool back);
static void playEnvBeep(EnvironmentStatus status);
static void playFinishedBeep();
static void beepShort();
static void beepLong();
static void playButtonBeep(); // 新增按键反馈函数

// ==================== 蜂鸣器控制宏定义 ====================
#define BEEP_ON()    digitalWrite(BUZZER_PIN, LOW)   // 低电平触发
#define BEEP_OFF()   digitalWrite(BUZZER_PIN, HIGH)  // 高电平关闭

// ==================== 非阻塞蜂鸣器控制 ====================
// 非阻塞蜂鸣器状态
typedef struct {
    bool active;
    uint32_t startTime;
    uint16_t duration;
    uint16_t freq;
} BuzzerTask;

static BuzzerTask buzzerTask = {false};

// 非阻塞蜂鸣器更新（在主循环中调用）
void Buzzer_Update() {
    if (!buzzerTask.active) return;
    
    uint32_t now = millis();
    uint32_t elapsed = now - buzzerTask.startTime;
    
    if (elapsed < buzzerTask.duration) {
        // 产生PWM信号（非阻塞）
        static uint32_t lastToggle = 0;
        uint32_t period = 1000000 / buzzerTask.freq;
        
        if (now - lastToggle > (period / 2000)) {  // 转换为毫秒
            digitalWrite(BUZZER_PIN, !digitalRead(BUZZER_PIN));
            lastToggle = now;
        }
    } else {
        // 播放完成
        BEEP_OFF();
        buzzerTask.active = false;
    }
}

// 开始播放蜂鸣音
void Buzzer_Play(uint16_t freq, uint16_t duration) {
    buzzerTask.active = true;
    buzzerTask.startTime = millis();
    buzzerTask.freq = freq;
    buzzerTask.duration = duration;
    BEEP_ON();
}

// ==================== 蜂鸣器控制函数 ====================
/**
 * 控制蜂鸣器发出特定频率的声音
 * @param freq 频率（Hz）
 * @param duration 持续时间（毫秒）
 */
static void buzzerTone(uint16_t freq, uint16_t duration) {
    // 计算周期（微秒）
    uint32_t period = 1000000 / freq;
    uint32_t halfPeriod = period / 2;
    uint32_t cycles = (duration * 1000) / period;
    
    for (uint32_t i = 0; i < cycles; i++) {
        BEEP_ON();
        delayMicroseconds(halfPeriod);
        BEEP_OFF();
        delayMicroseconds(halfPeriod);
    }
}

/**
 * 短促按键反馈
 */
static void playButtonBeep() {
    BEEP_ON();
    delay(20);
    BEEP_OFF();
}

/**
 * 短蜂鸣
 */
static void beepShort() {
    BEEP_ON();    // LOW
    delay(30);
    BEEP_OFF();   // HIGH
}

/**
 * 长蜂鸣
 */
static void beepLong() {
    BEEP_ON();    // LOW
    delay(200);
    BEEP_OFF();   // HIGH
}

/**
 * 根据环境状态播放对应的蜂鸣提示
 */
static void playEnvBeep(EnvironmentStatus status) {
    // 确保蜂鸣器先关闭
    BEEP_OFF();
    
    switch (status) {
        case STATUS_NO_PERSON:
            // 无人：一声长蜂鸣
            beepLong();
            break;
        case STATUS_NOISE_HIGH:
            // 噪音大：两声短蜂鸣
            beepShort();
            delay(100);
            beepShort();
            break;
        case STATUS_LIGHT_DARK:
            // 光线暗：三声短蜂鸣
            beepShort();
            delay(100);
            beepShort();
            delay(100);
            beepShort();
            break;
        case STATUS_MULTIPLE:
            // 多种问题：两声长蜂鸣
            beepLong();
            delay(200);
            beepLong();
            break;
        case STATUS_GOOD:
            // 环境良好，不发出声音
            break;
    }
    
    // 确保函数结束时蜂鸣器是关闭的
    BEEP_OFF();
}

/**
 * 播放完成状态的欢快蜂鸣提示
 */
static void playFinishedBeep() {
    BEEP_OFF();  // 确保起始状态
    
    for (int i = 0; i < 3; i++) {
        BEEP_ON();   // 响
        delay(80);
        BEEP_OFF();  // 停
        delay(80);
        BEEP_ON();   // 响
        delay(40);
        BEEP_OFF();  // 停
        delay(120);
    }
    
    BEEP_OFF();  // 确保最后关闭
}

// ==================== 传感器辅助函数 ====================
/**
 * 计算数组平均值（用于传感器滤波）
 */
static uint16_t calculateAverage(uint16_t* samples, uint8_t count) {
    uint32_t sum = 0;
    for (uint8_t i = 0; i < count; i++) {
        sum += samples[i];
    }
    return (uint16_t)(sum / count);
}

// ==================== 环境检测函数 ====================
/**
 * 检测环境状态
 * 只在倒计时状态执行，并且只有启用的传感器才会检测
 */
static EnvironmentStatus checkEnvironment() {
    // 只有在倒计时状态才进行检测
    if (currentState != STATE_COUNTING) {
        return STATUS_GOOD;
    }
    
    // 检测是否有人（PIR传感器）
    bool noPerson = false;
    if (hasPirSensor) {
        noPerson = !digitalRead(PIR_PIN);  // 假设有PIR传感器输出低电平表示无人
        // 只输出PIR传感器的关键信息
        if (!noPerson) {
            Serial.println("[PIR] Person detected");
        } else {
            Serial.println("[PIR] No person detected");
        }
    }
    
    // 检测噪音水平（只有启用时才检测）
    bool noiseHigh = false;
    if (userConfig.noiseEnabled) {
        int soundValue = analogRead(SOUND_PIN);
        noiseHigh = (soundValue > SOUND_THRESHOLD);
    }
    
    // 检测光线水平（只有启用时才检测）
    bool lightDark = false;
    if (userConfig.lightEnabled) {
        int lightValue = analogRead(LIGHT_PIN);
        lightDark = (lightValue < LIGHT_MIN);
    }
    
    // 环境状态判断逻辑
    EnvironmentStatus status;
    if (noPerson) {
        status = STATUS_NO_PERSON;
    } else if (userConfig.noiseEnabled && userConfig.lightEnabled && noiseHigh && lightDark) {
        status = STATUS_MULTIPLE;
    } else if (userConfig.noiseEnabled && noiseHigh) {
        status = STATUS_NOISE_HIGH;
    } else if (userConfig.lightEnabled && lightDark) {
        status = STATUS_LIGHT_DARK;
    } else {
        status = STATUS_GOOD;
    }
    
    // 更新全局环境状态
    envStatus = status;
    
    return status;
}

// ==================== 倒计时处理函数 ====================
/**
 * 更新倒计时时间
 * 每秒更新一次，时间到0时切换到完成状态
 */
static void updateCountdown() {
    if (currentState != STATE_COUNTING) return;
    
    // 检查更新间隔
    if (millis() - lastCountdownUpdate < COUNTDOWN_INTERVAL) return;
    lastCountdownUpdate = millis();
    
    uint32_t now = millis();
    remainingTime = userConfig.studyTime - (now - startTime) / 1000;
    
    // 时间结束处理
    if (remainingTime <= 0) {
        remainingTime = 0;
        currentState = STATE_FINISHED;
        playFinishedBeep();
        dirty = true;
    } else {
        dirty = true;  // 每秒更新一次显示
    }
}

// ==================== 状态处理函数 ====================
/**
 * 初始化状态处理
 */
static void handleInitState(bool ok, bool back) {
    // 仅在按键按下时有响应
    if (ok) {
        currentState = STATE_SELECT_TIME;
        dirty = true;
        
        // 蜂鸣器短促提示音，确认状态切换
        pinMode(BUZZER_PIN, OUTPUT);
        BEEP_ON();
        delay(50);
        BEEP_OFF();
    }
}

/**
 * 时间选择状态处理
 */
static void handleTimeSelectState(bool ok, bool back) {
    // 按键按下的边缘触发检测
    static bool lastOkPressed = false;
    static bool lastBackPressed = false;
    
    // 获取当前时间（分钟）
    uint16_t currentMinutes = secondsToMinutes(userConfig.studyTime);
    
    if (ok && !lastOkPressed) {
        // 确认选择，进入环境选择
        currentState = STATE_SELECT_ENV;
        dirty = true;
        
        // 蜂鸣器短促提示音
        playButtonBeep();
        
        lastOkPressed = true;
    } else if (!ok && lastOkPressed) {
        lastOkPressed = false;
    }
    
    if (back && !lastBackPressed) {
        // 调整时间：+5分钟，循环
        currentMinutes += 5;
        if (currentMinutes > 60) {
            currentMinutes = 5;
        }
        
        // 转换回秒并保存
        userConfig.studyTime = minutesToSeconds(currentMinutes);
        
        // 播放按键反馈
        playButtonBeep();
        
        // 重新显示
        dirty = true;
        
        lastBackPressed = true;
    } else if (!back && lastBackPressed) {
        lastBackPressed = false;
    }
}

/**
 * 环境选择状态处理
 */
static void handleEnvSelectState(bool ok, bool back) {
    if (ok) {
        // 确认选择，开始倒计时
        currentState = STATE_COUNTING;
        remainingTime = userConfig.studyTime;
        startTime = millis();
        dirty = true;
        
        // 添加确认开始时的短促蜂鸣提示音
        BEEP_ON();
        delay(40);
        BEEP_OFF();
    } else if (back) {
        // 切换环境检测模式（4种模式循环）
        if (userConfig.noiseEnabled && userConfig.lightEnabled) {
            // 全开 → 仅光线
            userConfig.noiseEnabled = false;
            userConfig.lightEnabled = true;
        } else if (!userConfig.noiseEnabled && userConfig.lightEnabled) {
            // 仅光线 → 仅噪音
            userConfig.noiseEnabled = true;
            userConfig.lightEnabled = false;
        } else if (userConfig.noiseEnabled && !userConfig.lightEnabled) {
            // 仅噪音 → 全关
            userConfig.noiseEnabled = false;
            userConfig.lightEnabled = false;
        } else {
            // 全关 → 全开
            userConfig.noiseEnabled = true;
            userConfig.lightEnabled = true;
        }
        dirty = true;
    }
}

/**
 * 倒计时状态处理
 */
static void handleCountingState(bool ok, bool back) {
    // 在倒计时状态下，确认键和返回键无特殊功能
    // 可以留作扩展（如暂停功能）
    
    // 更新倒计时
    updateCountdown();
    
    // 检测环境并处理异常提示
    EnvironmentStatus status = checkEnvironment();
    
    // 只有异常状态且达到警告间隔时才播放蜂鸣提示
    if (status != STATUS_GOOD && millis() - lastWarningTime > WARNING_INTERVAL) {
        playEnvBeep(status);
        lastWarningTime = millis();
        // 标记需要更新显示
        dirty = true;
    }
}

/**
 * 完成状态处理
 */
static void handleFinishedState(bool ok, bool back) {
    if (ok) {
        // 重新开始：回到时间选择
        currentState = STATE_SELECT_TIME;
        dirty = true;
    } else if (back) {
        // 返回主页
        currentState = STATE_INIT;
        dirty = true;
    }
}

// ==================== 公共接口函数实现 ====================
/**
 * 初始化自律立方系统
 */
void SelfDiscipline_Init() {
    // 初始化蜂鸣器引脚为高电平（不响）
    pinMode(BUZZER_PIN, OUTPUT);
    BEEP_OFF();  // 确保蜂鸣器关闭
    
    // 初始化传感器引脚
    pinMode(PIR_PIN, INPUT);
    pinMode(LIGHT_PIN, INPUT);
    pinMode(SOUND_PIN, INPUT);
    
    // 初始化状态变量
    currentState = STATE_INIT;
    userConfig.studyTime = DEFAULT_STUDY_TIME * 60;  // 转换为秒
    userConfig.noiseEnabled = true;
    userConfig.lightEnabled = true;
    
    // 记录PIR传感器状态
    Serial.print("[PIR] Sensor - enabled: ");
    Serial.println(hasPirSensor);
    if (hasPirSensor) {
        Serial.println("[PIR] Human detection is active");
    }
    
    remainingTime = 0;
    startTime = 0;
    appStartTime = millis();
    envStatus = STATUS_GOOD;
    lastEnvStatus = STATUS_GOOD;
    dirty = true;
    lastWarningTime = 0;
}

/**
 * 重置系统到初始状态
 */
void SelfDiscipline_Reset() {
    SelfDiscipline_Init();  // 重用初始化逻辑
    dirty = true;
}

/**
 * 自律立方主循环函数
 */
void SelfDiscipline_Loop(bool ok, bool back) {
    // 更新非阻塞蜂鸣器状态
    Buzzer_Update();
    
    // 检查双键同时按的情况（系统重置）
    if (ok && back) {
        SelfDiscipline_Reset();
        
        // 发出短促蜂鸣提示音，表示系统已重置
        BEEP_ON();
        delay(40);
        BEEP_OFF();
        
        // 提前返回，避免处理单个按键
        return;
    }
    
    // 根据当前状态调用对应的处理函数
    switch (currentState) {
        case STATE_INIT:
            handleInitState(ok, back);
            break;
        case STATE_SELECT_TIME:
            handleTimeSelectState(ok, back);
            break;
        case STATE_SELECT_ENV:
            handleEnvSelectState(ok, back);
            break;
        case STATE_COUNTING:
            handleCountingState(ok, back);
            break;
        case STATE_FINISHED:
            handleFinishedState(ok, back);
            break;
        default:
            // 未知状态，重置系统
            currentState = STATE_INIT;
            dirty = true;
            break;
    }
}

// ==================== 状态获取函数实现 ====================
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

uint32_t SelfDiscipline_GetElapsed() {
    return (millis() - appStartTime) / 1000;
}

bool SelfDiscipline_IsDirty() {
    return dirty;
}

void SelfDiscipline_ClearDirty() {
    dirty = false;
}