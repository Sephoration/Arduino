// 自律立方硬件引脚定义与参数配置文件
// 集中管理所有硬件组件的引脚定义和参数阈值，方便后续修改和维护

#ifndef PINDEFINES_H
#define PINDEFINES_H

// ==================== 按键引脚定义 ====================
#define BUTTON_OK          PA5     // 确认按键 (数字输入，内置上拉)
#define BUTTON_PIN         PA15    // 返回按键 (数字输入，内置上拉)

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
#define LIGHT_MIN           100     // 最小合适光照值 (模拟值，范围0-1023，可根据环境调整)
#define SOUND_THRESHOLD     500     // 噪音检测阈值 (模拟值，范围0-1023，可根据环境调整)

// ==================== 蜂鸣器参数配置 ====================
#define WARNING_DURATION    500     // 警告蜂鸣持续时间 (ms)

// 不同环境状态的蜂鸣频率配置 (Hz)
#define NO_PERSON_BEEP_FREQ     500   // 无人状态蜂鸣频率
#define NOISE_HIGH_BEEP_FREQ    1000  // 噪音过大蜂鸣频率
#define LIGHT_DARK_BEEP_FREQ    1500  // 光线过暗蜂鸣频率
#define MULTIPLE_BEEP_FREQ      2000  // 多种问题蜂鸣频率
#define FINISHED_BEEP_FREQ      800   // 完成状态蜂鸣频率

// 蜂鸣节奏参数配置 (ms)
#define SHORT_BEEP_DURATION     100   // 短蜂鸣持续时间
#define LONG_BEEP_DURATION      500   // 长蜂鸣持续时间
#define BEEP_INTERVAL           200   // 蜂鸣间隔时间 (ms)

#endif /* PINDEFINES_H */