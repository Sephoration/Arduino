// OLED_Display.h - OLED显示屏控制类
// 封装SSD1306 OLED显示屏的初始化、控制和绘制功能
// 提供简洁的API接口，方便上层应用使用

#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ==================== OLED显示控制类 ====================
/**
 * @brief OLED显示控制类
 * @details 封装了SSD1306 OLED显示屏的初始化、控制和绘制功能
 *          提供简洁的API接口，方便上层应用使用
 */
class OLED_Display {
  public:
    /**
     * @brief 构造函数
     * @param width 屏幕宽度，默认128像素
     * @param height 屏幕高度，默认64像素
     * @param resetPin 复位引脚，默认-1（不使用复位引脚）
     * @details 初始化OLED显示控制类，设置屏幕尺寸和复位引脚
     */
    OLED_Display(uint8_t width = 128, uint8_t height = 64, int8_t resetPin = -1);
    
    /**
     * @brief 初始化显示设备
     * @param i2cAddress I2C地址，默认0x3C
     * @return 初始化是否成功
     * @details 初始化SSD1306 OLED显示屏，配置I2C通信
     */
    bool begin(uint8_t i2cAddress = 0x3C);
    
    /**
     * @brief 清空显示缓存
     * @details 清空OLED显示屏的显示缓存，不立即更新屏幕
     */
    void clear();                   // 清空显示缓存
    
    /**
     * @brief 更新显示内容
     * @details 将显示缓存中的内容刷新到OLED屏幕上
     */
    void update();                  // 更新显示内容
    
    /**
     * @brief 显示边框
     * @details 在OLED屏幕上绘制一个边框
     */
    void showBorder();              // 显示边框
    
    /**
     * @brief 显示启动画面
     * @details 在OLED屏幕上显示Focus Cube的启动画面
     */
    void showBootScreen();          // 显示启动画面
    
    /**
     * @brief 设置文字大小
     * @param size 文字大小，范围1-8
     * @details 设置后续绘制文字的大小
     */
    void setTextSize(uint8_t size);     // 设置文字大小
    
    /**
     * @brief 设置文字颜色
     * @param color 文字颜色，SSD1306_WHITE或SSD1306_BLACK
     * @details 设置后续绘制文字的颜色
     */
    void setTextColor(uint16_t color);  // 设置文字颜色
    
    /**
     * @brief 设置光标位置
     * @param x X坐标
     * @param y Y坐标
     * @details 设置后续绘制操作的起始位置
     */
    void setCursor(int16_t x, int16_t y); // 设置光标位置
    
    /**
     * @brief 打印文本
     * @param text 要打印的文本
     * @details 在当前光标位置打印文本
     */
    void print(const String &text);     // 打印文本
    
    /**
     * @brief 填充矩形
     * @param x X坐标
     * @param y Y坐标
     * @param w 宽度
     * @param h 高度
     * @param color 填充颜色
     * @details 在指定位置填充一个矩形区域
     */
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color); // 填充矩形
    
    /**
     * @brief 绘制矩形边框
     * @param x X坐标
     * @param y Y坐标
     * @param w 宽度
     * @param h 高度
     * @param color 边框颜色
     * @details 在指定位置绘制一个矩形边框
     */
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color); // 绘制矩形边框
    
    /**
     * @brief 填充整个屏幕
     * @param color 填充颜色
     * @details 用指定颜色填充整个OLED屏幕
     */
    void fillScreen(uint16_t color);    // 填充整个屏幕
    
    /**
     * @brief 获取初始化状态
     * @return 初始化是否成功
     * @details 返回OLED显示屏的初始化状态
     */
    bool isInitialized() { return _initialized; }
    
  private:
    Adafruit_SSD1306 _display;      // SSD1306显示对象
    bool _initialized = false;      // 初始化标志
    uint8_t _width;                 // 屏幕宽度
    uint8_t _height;                // 屏幕高度
};

#endif