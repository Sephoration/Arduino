// SelfDiscipline.h - 自律立方功能定义
// 定义自律立方的系统状态、环境状态、配置结构体和核心函数接口

#ifndef SELFDISCIPLINE_H
#define SELFDISCIPLINE_H

#include "OLED_Display.h"

// ==================== 系统状态枚举 ====================
/**
 * @brief 自律立方系统状态枚举
 * @details 定义了自律立方的不同运行状态，用于状态机管理
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
 * @brief 环境状况枚举
 * @details 定义了不同的环境状态，用于评估学习环境质量
 */
typedef enum {
    STATUS_GOOD,          // 环境良好
    STATUS_NO_PERSON,     // 无人（PIR传感器未检测到人体）
    STATUS_NOISE_HIGH,    // 噪音大（超过预设阈值）
    STATUS_LIGHT_DARK,    // 光线暗（低于预设阈值）
    STATUS_MULTIPLE       // 多种环境问题同时存在
} EnvironmentStatus;

// ==================== 配置结构体 ====================
/**
 * @brief 用户配置结构体
 * @details 存储用户设置的学习参数和环境检测选项
 */
typedef struct {
    uint16_t studyTime;     // 学习时间（秒）
    bool noiseEnabled;      // 是否开启噪音检测
    bool lightEnabled;      // 是否开启光线检测
} UserConfig;

// ==================== 函数声明 ====================

/**
 * @brief 初始化自律立方系统
 * @details 初始化所有硬件资源、状态变量和配置参数
 */
void SelfDiscipline_Init(void);

/**
 * @brief 重置自律立方系统
 * @details 将系统状态重置为初始状态，重新初始化所有参数
 */
void SelfDiscipline_Reset(void);

/**
 * @brief 自律立方主循环函数
 * @param ok 确认按键是否被按下
 * @param back 返回按键是否被按下
 * @details 处理系统状态转换和用户输入，是系统的核心控制函数
 */
void SelfDiscipline_Loop(bool ok, bool back);

/**
 * @brief 自律立方绘制函数
 * @param oled OLED显示屏对象引用
 * @details 根据当前系统状态绘制相应的界面
 */
void SelfDiscipline_Draw(OLED_Display& oled);

/**
 * @brief 获取系统运行时间
 * @return 系统运行时间（秒）
 * @details 返回自律立方从启动到当前的运行时间
 */
uint32_t SelfDiscipline_GetElapsed(void);

/**
 * @brief 获取当前系统状态
 * @return 当前系统状态
 * @details 返回自律立方的当前运行状态
 */
SystemState SelfDiscipline_GetState(void);

/**
 * @brief 获取用户配置
 * @return 用户配置结构体
 * @details 返回当前的用户配置参数
 */
UserConfig SelfDiscipline_GetConfig(void);

/**
 * @brief 获取剩余时间
 * @return 剩余时间（秒）
 * @details 返回倒计时状态下的剩余时间
 */
uint32_t SelfDiscipline_GetRemainingTime(void);

/**
 * @brief 获取环境状态
 * @return 环境状态
 * @details 返回当前检测到的环境状态
 */
EnvironmentStatus SelfDiscipline_GetEnvStatus(void);

/**
 * @brief 检查是否需要重绘界面
 * @return 是否需要重绘
 * @details 检查脏标记，判断是否需要更新显示屏内容
 */
bool SelfDiscipline_IsDirty(void);

/**
 * @brief 清除脏标记
 * @details 清除脏标记，表示界面已经更新完成
 */
void SelfDiscipline_ClearDirty(void);

#endif /* SELFDISCIPLINE_H */