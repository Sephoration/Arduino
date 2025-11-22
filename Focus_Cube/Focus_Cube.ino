// Focus_Cube.ino - 主程序文件
#include "OLED_Display.h"
#include "AppType.h"  // 应用类型定义头文件

// ==================== 硬件引脚定义 ====================
#define BUTTON_PA0 0    // 确认按键引脚
#define BUTTON_PA3 3    // 返回/切换按键引脚

// ==================== 类型定义 ====================
// 应用函数指针类型定义
typedef void (*AppInitFunc)();                         // 应用初始化函数类型
typedef void (*AppLoopFunc)(bool ok, bool back);       // 应用循环函数类型  
typedef void (*AppDrawFunc)(OLED_Display&);            // 应用绘制函数类型

// 应用结构体定义
struct App { 
  AppInitFunc init;    // 初始化函数指针
  AppLoopFunc loop;    // 循环处理函数指针  
  AppDrawFunc draw;    // 绘制函数指针
};

// ==================== 全局变量定义 ====================
App appRouter[APP_COUNT];          // 应用路由器数组
AppID currentApp    = APP_HOME;    // 当前运行的应用

OLED_Display oled;                 // OLED显示对象
bool     lastPA0    = HIGH;        // PA0按键上次状态
bool     lastPA3    = HIGH;        // PA3按键上次状态
uint32_t lastDeb    = 0;           // 防抖时间记录

// ==================== 应用注册函数 ====================
// 注册应用到路由器
static void registerApp(AppID id, AppInitFunc i, AppLoopFunc l, AppDrawFunc d) {
  if (id < APP_COUNT) appRouter[id] = {i, l, d};
}

// ==================== 应用切换函数 ====================
// 切换到指定应用
void switchApp(AppID id) {
  currentApp = id;  // 更新当前应用ID
  
  // 调用新应用的初始化函数
  if (appRouter[id].init) appRouter[id].init();
}

// ==================== 按键读取函数 ====================
// 时间戳防抖采键函数
static void readKeys(bool &ok, bool &back) {
  ok = back = false;  // 重置按键状态
  
  // 读取当前按键状态
  bool nowPA0 = digitalRead(BUTTON_PA0);
  bool nowPA3 = digitalRead(BUTTON_PA3);
  
  // 防抖处理 - 50ms内不重复检测
  if (millis() - lastDeb < 50) return;
  
  // 检测PA0按键按下（下降沿）
  if (nowPA0 != lastPA0 && nowPA0 == LOW) { 
    ok   = true; 
    lastDeb = millis();  // 更新防抖时间
  }
  
  // 检测PA3按键按下（下降沿）
  if (nowPA3 != lastPA3 && nowPA3 == LOW) { 
    back = true; 
    lastDeb = millis();  // 更新防抖时间
  }
  
  // 更新按键状态记录
  lastPA0 = nowPA0;
  lastPA3 = nowPA3;
}

// ==================== Arduino setup函数 ====================
// 系统初始化函数
void setup() {
  // 初始化串口通信
  Serial.begin(115200);
  
  // 配置按键引脚为上拉输入模式
  pinMode(BUTTON_PA0, INPUT_PULLUP);
  pinMode(BUTTON_PA3, INPUT_PULLUP);

  // 初始化OLED显示
  if (!oled.begin()) { 
    Serial.println(F("OLED初始化失败")); 
    while (1);  // 初始化失败时停机
  }
  
  // 显示启动画面
  oled.showBootScreen();
  delay(1500);  // 显示1.5秒启动画面

  /* 注册系统必备的两个应用 */
  registerApp(APP_HOME, HomePage_init, HomePage_loop, HomePage_draw);
  registerApp(APP_LIST, ListPage_init, ListPage_loop, ListPage_draw);

  switchApp(APP_HOME);  // 上电默认显示主页
}

// ==================== Arduino loop函数 ====================
// 主循环函数
void loop() {
  bool ok, back;  // 按键状态变量
  
  // 读取按键输入（带防抖）
  readKeys(ok, back);

  // 调用当前应用的循环处理函数
  if (appRouter[currentApp].loop)
    appRouter[currentApp].loop(ok, back);

  // 渲染循环 - 30fps帧率控制
  static uint32_t lastDraw;  // 上次渲染时间
  if (millis() - lastDraw >= 33) {      // 33ms ≈ 30fps
    // 调用当前应用的绘制函数
    if (appRouter[currentApp].draw)
      appRouter[currentApp].draw(oled);
    lastDraw = millis();  // 更新渲染时间
  }
}