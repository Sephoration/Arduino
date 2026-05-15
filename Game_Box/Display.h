#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64

void Display_Init(void);
void Display_Clear(void);
void Display_Update(void);
void Display_DrawPixel(int16_t x, int16_t y, uint16_t color);
void Display_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void Display_DrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void Display_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void Display_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void Display_FillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void Display_DrawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
void Display_DrawText(int16_t x, int16_t y, const char* text, uint16_t color, uint8_t size);
void Display_DrawTextCenter(int16_t y, const char* text, uint16_t color, uint8_t size);
void Display_DrawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
void Display_Invert(bool inv);
Adafruit_SSD1306* Display_GetRef(void);

#endif
