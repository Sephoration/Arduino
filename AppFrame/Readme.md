AppFrame 嵌入式应用框架 - 使用说明
======================================

项目概述
--------
这是一个基于Arduino的嵌入式应用框架，支持自动应用注册和动态菜单生成。
适用于小型OLED显示设备和按键交互的场景。

文件结构
--------
AppFrame.ino      - 主程序文件
AppType.h         - 应用类型定义和注册框架
OLED_Display.h/cpp - OLED显示驱动
HomePage_*.cpp    - 主页应用
ListPage_*.cpp    - 列表页面应用

【需要修改的地方标注】
====================

1. 启动画面修改
---------------
文件：OLED_Display.cpp
函数：showBootScreen()

需要修改的内容：
- 显示文本："ToolBox" 
- 显示位置：setCursor(25, 25)
- 边框显示：drawRect(0, 0, _width, _height, SSD1306_WHITE)

修改示例：
void OLED_Display::showBootScreen(){
  // ... 其他代码不变
  setCursor(30, 25);           // 修改X坐标调整水平位置
  print("MyDevice");           // 修改显示文本
  // 如需去掉边框，注释掉下面这行：
  // drawRect(0, 0, _width, _height, SSD1306_WHITE);
}

2. 主页显示内容修改
-----------------
文件：HomePage_draw.cpp
函数：HomePage_draw()

需要修改的内容：
- 主标题："ToolBox" (第20行)
- 副标题："Press to start" (第26行)
- 时间显示格式 (第30-33行)

3. 硬件引脚定义修改
-----------------
文件：AppFrame.ino

需要修改的内容：
- 确认按键引脚：BUTTON_PA0 (第4行)
- 返回按键引脚：BUTTON_PA3 (第5行)

【添加新功能步骤】
================

步骤1：在AppType.h中添加应用ID
--------------------------------
在enum AppID中添加新ID（第10-15行区域）：

enum AppID : uint8_t { 
    APP_HOME = 0,
    APP_LIST,
    // ！！！在这里添加新应用ID ！！！
    APP_POMODORO,    // 示例：番茄钟应用
    APP_COUNT
};

步骤2：声明应用函数
-------------------
在AppType.h中添加函数声明（第25-30行区域）：

// ！！！在这里声明新应用的函数 ！！！
void Pomodoro_init();
void Pomodoro_loop(bool ok, bool back);
void Pomodoro_draw(OLED_Display& oled);

步骤3：注册应用到系统
--------------------
在AppType.h的APP_REGISTRY中添加（第35-40行区域）：

#define APP_REGISTRY \
    APP_ENTRY(APP_HOME, F("Home"), HomePage_init, HomePage_loop, HomePage_draw) \
    APP_ENTRY(APP_LIST, F("List"), ListPage_init, ListPage_loop, ListPage_draw) \
    // ！！！在这里注册新应用 ！！！
    APP_ENTRY(APP_POMODORO, F("Pomodoro"), Pomodoro_init, Pomodoro_loop, Pomodoro_draw)

步骤4：创建应用文件
------------------
创建两个新文件：
- Pomodoro_page.cpp    (应用逻辑)
- Pomodoro_draw.cpp    (显示渲染)

按照现有模板实现三个函数：
- Pomodoro_init()    - 初始化
- Pomodoro_loop()    - 逻辑处理  
- Pomodoro_draw()    - 界面绘制

【重要注意事项】
==============

1. 字符串使用F()宏
------------------
所有显示字符串请使用F()宏包裹，节省RAM：
正确：print(F("Hello World"))
错误：print("Hello World")

2. 脏标记管理
-------------
每个应用需要维护dirty标志：
- 数据变化时设置 dirty = true
- 绘制完成后调用 clearDirty()
- 绘制前检查 isDirty()

3. 按键处理规范
---------------
- ok参数：确认按键（PA0）
- back参数：返回/切换按键（PA3）
- 在loop函数中处理按键逻辑

4. 性能优化
----------
- 帧率已设置为15fps，平衡性能和流畅度
- 使用PROGMEM存储字符串
- 非必要不重绘（依赖脏标记）

5. 内存管理
----------
- 避免在栈上分配大数组
- 使用静态变量保持状态
- 及时清理不需要的资源

【编译和调试】
============

1. 依赖库：
   - Adafruit GFX Library
   - Adafruit SSD1306

2. 开发板：Arduino Nano/UNO

3. 串口调试：波特率115200

4. 常见问题：
   - 编译错误：检查所有函数声明是否匹配
   - 显示异常：检查OLED地址(0x3C)和接线
   - 按键无响应：检查引脚模式和上拉电阻

【扩展建议】
==========

1. 添加新应用时，参考现有HomePage/ListPage模板
2. 复杂功能可拆分为多个状态
3. 长时间运行任务注意避免阻塞主循环
4. 需要持久化数据可考虑EEPROM存储

版本信息
--------
当前版本：AppFrame v2.0
框架特性：自动注册、内存优化、易于扩展
维护说明：核心框架稳定，主要修改应用层功能