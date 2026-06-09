#include "Display.h"
#include "PinDefines.h"
#include <Wire.h>

static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void Display_Init(void) {
    Wire.begin();
    if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        // 初始化失败时挂起，实际可接串口调试
        for(;;);
    }
    display.clearDisplay();
    display.display();
    display.setTextWrap(false);
}

void Display_Clear(void) {
    display.clearDisplay();
}

void Display_Update(void) {
    display.display();
}

void Display_DrawPixel(int16_t x, int16_t y, uint16_t color) {
    display.drawPixel(x, y, color);
}

void Display_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    display.drawLine(x0, y0, x1, y1, color);
}

void Display_DrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    display.drawRect(x, y, w, h, color);
}

void Display_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    display.fillRect(x, y, w, h, color);
}

void Display_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    display.drawCircle(x0, y0, r, color);
}

void Display_FillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    display.fillCircle(x0, y0, r, color);
}

void Display_DrawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size) {
    display.drawChar(x, y, c, color, bg, size);
}

void Display_DrawText(int16_t x, int16_t y, const char* text, uint16_t color, uint8_t size) {
    display.setTextSize(size);
    display.setTextColor(color);
    display.setCursor(x, y);
    display.print(text);
}

void Display_DrawTextCenter(int16_t y, const char* text, uint16_t color, uint8_t size) {
    int16_t len = strlen(text);
    int16_t w = len * 6 * size;
    int16_t x = (SCREEN_WIDTH - w) / 2;
    Display_DrawText(x, y, text, color, size);
}

void Display_DrawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color) {
    display.drawBitmap(x, y, bitmap, w, h, color);
}

void Display_Invert(bool inv) {
    display.invertDisplay(inv);
}

Adafruit_SSD1306* Display_GetRef(void) {
    return &display;
}
