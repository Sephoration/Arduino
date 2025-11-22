#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  Serial.println("🚀 Focus Cube - Starting...");
  
  // 初始化OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ OLED init failed!");
    return;
  }
  
  Serial.println("✅ OLED initialized!");
  
  // 显示启动画面
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println("Focus");
  display.setCursor(20, 20);
  display.println("Cube");
  
  display.setTextSize(1);
  display.setCursor(10, 45);
  display.println("自律方块");
  display.setCursor(25, 55);
  display.println("Ready!");
  
  display.display();
  Serial.println("📺 Display should show: Focus Cube");
}

void loop() {
  // 更新运行时间
  display.fillRect(80, 55, 40, 8, SSD1306_BLACK); // 清除时间区域
  display.setCursor(80, 55);
  display.print(millis() / 1000);
  display.print("s");
  display.display();
  
  delay(1000);
}