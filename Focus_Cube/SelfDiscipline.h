// SelfDiscipline.h - 自律立方功能定义
// 定义自律立方的系统状态、环境状态、配置结构体和核心函数接口
// 这是整个项目的核心头文件，所有其他文件都依赖它

#ifndef SELFDISCIPLINE_H
#define SELFDISCIPLINE_H

#include <Arduino.h>
#include "OLED_Display.h"

// ==================== 系统状态枚举 ====================
/**
 * 自律立方系统状态枚举
 * 定义了系统的5个核心运行状态
 */
typedef enum {
    STATE_INIT,           // 初始化状态，显示主页
    STATE_SELECT_TIME,    // 时间选择状态，设置学习时长
    STATE_SELECT_ENV,     // 环境检测选择状态，配置环境检测选项
    STATE_COUNTING,       // 倒计时状态，正在进行学习计时
    STATE_FINISHED        // 完成状态，学习时间结束
} SystemState;

// ==================== 环境状况枚举 ====================
/**
 * 环境状况枚举
 * 定义了5种不同的环境状态，用于评估学习环境质量
 */
typedef enum {
    STATUS_GOOD,          // 环境良好，适合学习
    STATUS_NO_PERSON,     // 无人（PIR传感器未检测到人体）
    STATUS_NOISE_HIGH,    // 噪音过大（超过预设阈值）
    STATUS_LIGHT_DARK,    // 光线过暗（低于预设阈值）
    STATUS_MULTIPLE       // 多种环境问题同时存在
} EnvironmentStatus;

// ==================== 用户配置结构体 ====================
/**
 * 用户配置结构体
 * 存储用户设置的学习参数和环境检测选项
 */
typedef struct {
    uint16_t studyTime;     // 学习时间（秒），范围：5-60分钟
    bool noiseEnabled;      // 是否开启噪音检测
    bool lightEnabled;      // 是否开启光线检测
} UserConfig;

// ==================== 核心函数声明 ====================
// 注意：这些函数将在 SelfDiscipline_page.cpp 中实现

/**
 * 初始化自律立方系统
 * 在系统启动时调用，初始化所有状态和配置
 */
void SelfDiscipline_Init(void);

/**
 * 重置自律立方系统
 * 将系统恢复到初始状态，用于重新开始或错误恢复
 */
void SelfDiscipline_Reset(void);

/**
 * 自律立方主循环函数
 * 必须在Arduino的loop()中定期调用
 * @param ok 确认按键是否被按下
 * @param back 返回按键是否被按下
 */
void SelfDiscipline_Loop(bool ok, bool back);

/**
 * 自律立方绘制函数
 * 根据当前状态绘制相应的界面
 * @param oled OLED显示屏对象引用
 */
void SelfDiscipline_Draw(OLED_Display& oled);

// ==================== 状态获取函数声明 ====================
// 这些函数提供对内部状态的只读访问

/**
 * 获取当前系统状态
 * @return 当前系统状态枚举值
 */
SystemState SelfDiscipline_GetState(void);

/**
 * 获取用户配置
 * @return 用户配置结构体副本
 */
UserConfig SelfDiscipline_GetConfig(void);

/**
 * 获取剩余学习时间
 * @return 剩余时间（秒），仅在倒计时状态下有效
 */
uint32_t SelfDiscipline_GetRemainingTime(void);

/**
 * 获取环境状态
 * @return 当前环境状态枚举值
 */
EnvironmentStatus SelfDiscipline_GetEnvStatus(void);

/**
 * 获取系统运行时间
 * @return 系统运行时间（秒），从启动开始计算
 */
uint32_t SelfDiscipline_GetElapsed(void);

/**
 * 检查是否需要重绘界面
 * @return true表示需要重绘，false表示不需要
 */
bool SelfDiscipline_IsDirty(void);

/**
 * 清除重绘标志
 * 在绘制完成后调用，表示界面已更新
 */
void SelfDiscipline_ClearDirty(void);

#endif /* SELFDISCIPLINE_H */