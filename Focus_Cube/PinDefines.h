// 自律立方硬件引脚定义与参数配置文件
// 集中管理所有硬件组件的引脚定义和参数阈值，方便后续修改和维护

#ifndef PINDEFINES_H
#define PINDEFINES_H

// ==================== 按键引脚定义 ====================
#define BUTTON_OK          PA5     // 确认按键 (数字输入，内置上拉)
#define BUTTON_PIN         PB0     // 返回按键 (数字输入，内置上拉)

// ==================== 传感器引脚定义 ====================
#define PIR_PIN             PA2     // 人体红外传感器 (数字输入)
#define LIGHT_PIN           PA3     // 光照传感器 (模拟输入)
#define SOUND_PIN           PA1     // 噪音传感器 (模拟输入)

// ==================== 蜂鸣器引脚定义 ====================
#define BUZZER_PIN          PA0     // 蜂鸣器 (PWM输出)

// ==================== 串口引脚定义 ====================
#define USART1_TX_PIN       PA10    // 串口发送引脚 (数字输出)
#define USART1_RX_PIN       PA9     // 串口接收引脚 (数字输入)

// ==================== OLED显示屏引脚定义 ====================
#define OLED_SCL_PIN        PB6     // OLED I2C时钟引脚 (数字输出)
#define OLED_SDA_PIN        PB7     // OLED I2C数据引脚 (数字输出)

// ==================== 传感器参数阈值 ====================
// 传感器阈值定义
#define LIGHT_MIN 10           // 光线最小值（低于此值视为光线暗）
#define SOUND_THRESHOLD 500       // 声音阈值（高于此值视为噪音大）

// ==================== 蜂鸣器参数配置 ====================
#define WARNING_DURATION    400     // 警告蜂鸣持续时间 (ms)

// 不同环境状态的蜂鸣频率配置 (Hz)
#define NO_PERSON_BEEP_FREQ     500   // 无人状态蜂鸣频率
#define NOISE_HIGH_BEEP_FREQ    1000  // 噪音过大蜂鸣频率
#define LIGHT_DARK_BEEP_FREQ    1500  // 光线过暗蜂鸣频率
#define MULTIPLE_BEEP_FREQ      2000  // 多种问题蜂鸣频率
#define FINISHED_BEEP_FREQ      800   // 完成状态蜂鸣频率

// 蜂鸣节奏参数配置 (ms)
#define SHORT_BEEP_DURATION     80    // 短蜂鸣持续时间
#define LONG_BEEP_DURATION      400   // 长蜂鸣持续时间
#define BEEP_INTERVAL           150   // 蜂鸣间隔时间 (ms)


// ==================== 时间参数配置 ====================
#define MIN_STUDY_TIME      5      // 最小学习时间（分钟）
#define MAX_STUDY_TIME      60     // 最大学习时间（分钟）
#define TIME_STEP           5      // 时间调整步进（分钟）
#define DEFAULT_STUDY_TIME  25     // 默认学习时间（分钟） - 标准番茄工作法定时


// ==================== 检测间隔参数 ====================
#define ENV_CHECK_INTERVAL  1000    // 环境检测间隔（ms）- 降低检测频率，减少传感器功耗
#define WARNING_INTERVAL    3000    // 警告间隔（ms，避免频繁报警）- 延长警告间隔，减少干扰
#define COUNTDOWN_INTERVAL  1000   // 倒计时更新间隔（ms）


// ==================== OLED显示参数 ====================
#define OLED_WIDTH          128    // OLED屏幕宽度（像素）
#define OLED_HEIGHT         64     // OLED屏幕高度（像素）
#define OLED_ADDRESS        0x3C   // OLED I2C地址

// 添加去抖动延迟参数
#define BUTTON_DEBOUNCE_MS 50      // 按键去抖动时间（毫秒）
#define BUTTON_LONG_PRESS_MS 500   // 长按判定时间

#endif /* PINDEFINES_H */