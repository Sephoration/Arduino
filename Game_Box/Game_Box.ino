#include "Display.h"
#include "Input.h"
#include "Sound.h"
#include "EEPROM_Save.h"
#include "Menu.h"
#include "Snake.h"
#include "Tetris.h"
#include "Brick.h"

typedef enum {
    STATE_BOOT = 0,
    STATE_MENU,
    STATE_PLAYING_SNAKE,
    STATE_POSTGAME_SNAKE,
    STATE_PLAYING_TETRIS,
    STATE_POSTGAME_TETRIS,
    STATE_PLAYING_BRICK,
    STATE_POSTGAME_BRICK
} SystemState_t;

static SystemState_t currentState;
static unsigned long bootTimer;

static void DrawBootScreen(void) {
    Display_Clear();
    Display_DrawTextCenter(10, "GAME BOX", SSD1306_WHITE, 2);
    Display_DrawLine(0, 30, 127, 30, SSD1306_WHITE);
    Display_DrawTextCenter(38, "Arduino", SSD1306_WHITE, 1);
    Display_DrawTextCenter(50, "Mini Console", SSD1306_WHITE, 1);

    // 简单进度条动画
    int16_t progress = ((millis() - bootTimer) * 128) / 1500;
    if (progress > 128) progress = 128;
    Display_FillRect(0, 62, progress, 2, SSD1306_WHITE);

    Display_Update();
}

void setup(void) {
    Display_Init();
    Input_Init();
    Sound_Init();
    EEPROM_Save_Init();

    Menu_Init();
    Snake_Init();
    Tetris_Init();
    Brick_Init();

    currentState = STATE_BOOT;
    bootTimer = millis();
}

void loop(void) {
    Input_Update();

    // ---- 状态更新 ----
    switch (currentState) {
        case STATE_BOOT:
            if (millis() - bootTimer > 1500) {
                currentState = STATE_MENU;
                Menu_Init();
                Sound_PlayStart();
            }
            break;

        case STATE_MENU: {
            Menu_Update();
            MenuState_t ms = Menu_GetState();
            if (ms == MENU_STATE_LAUNCH_SNAKE) {
                currentState = STATE_PLAYING_SNAKE;
                Snake_Reset();
                Sound_PlayStart();
            } else if (ms == MENU_STATE_LAUNCH_TETRIS) {
                currentState = STATE_PLAYING_TETRIS;
                Tetris_Reset();
                Sound_PlayStart();
            } else if (ms == MENU_STATE_LAUNCH_BRICK) {
                currentState = STATE_PLAYING_BRICK;
                Brick_Reset();
                Sound_PlayStart();
            } else if (ms == MENU_STATE_RESET_SCORES) {
                Menu_SetIdle();
            }
            break;
        }

        case STATE_PLAYING_SNAKE:
            Snake_Update();
            if (Snake_IsGameOver()) {
                currentState = STATE_POSTGAME_SNAKE;
            }
            break;

        case STATE_POSTGAME_SNAKE:
            if (Input_GetOkPressed()) {
                EEPROM_Save_SetHighScore(0, Snake_GetScore());
                currentState = STATE_MENU;
                Menu_Init();
            }
            break;

        case STATE_PLAYING_TETRIS:
            Tetris_Update();
            if (Tetris_IsGameOver()) {
                currentState = STATE_POSTGAME_TETRIS;
            }
            break;

        case STATE_POSTGAME_TETRIS:
            if (Input_GetOkPressed()) {
                EEPROM_Save_SetHighScore(1, Tetris_GetScore());
                currentState = STATE_MENU;
                Menu_Init();
            }
            break;

        case STATE_PLAYING_BRICK:
            Brick_Update();
            if (Brick_IsGameOver() || Brick_IsWin()) {
                currentState = STATE_POSTGAME_BRICK;
            }
            break;

        case STATE_POSTGAME_BRICK:
            if (Input_GetOkPressed()) {
                EEPROM_Save_SetHighScore(2, Brick_GetScore());
                currentState = STATE_MENU;
                Menu_Init();
            }
            break;
    }

    // ---- 画面渲染 ----
    switch (currentState) {
        case STATE_BOOT:
            DrawBootScreen();
            break;
        case STATE_MENU:
            Menu_Draw();
            break;
        case STATE_PLAYING_SNAKE:
        case STATE_POSTGAME_SNAKE:
            Snake_Draw();
            break;
        case STATE_PLAYING_TETRIS:
        case STATE_POSTGAME_TETRIS:
            Tetris_Draw();
            break;
        case STATE_PLAYING_BRICK:
        case STATE_POSTGAME_BRICK:
            Brick_Draw();
            break;
    }

    delay(10);
}
