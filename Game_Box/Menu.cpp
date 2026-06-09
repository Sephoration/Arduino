#include "Menu.h"
#include "Display.h"
#include "Input.h"
#include "Sound.h"
#include "EEPROM_Save.h"

#define MENU_ITEMS 4

static const char* menuNames[MENU_ITEMS] = {
    "Snake",
    "Tetris",
    "Brick",
    "Reset Score"
};

static int8_t cursor = 0;
static MenuState_t state = MENU_STATE_IDLE;
static unsigned long lastBlink = 0;
static bool blinkOn = true;

void Menu_Init(void) {
    cursor = 0;
    state = MENU_STATE_IDLE;
    blinkOn = true;
    lastBlink = millis();
}

void Menu_Update(void) {
    InputDir_t dir = Input_GetDirection();
    bool ok = Input_GetOkPressed();

    if (dir == DIR_DOWN) {
        cursor++;
        if (cursor >= MENU_ITEMS) cursor = 0;
        Sound_PlaySelect();
    } else if (dir == DIR_UP) {
        cursor--;
        if (cursor < 0) cursor = MENU_ITEMS - 1;
        Sound_PlaySelect();
    }

    if (ok) {
        Sound_PlayStart();
        switch (cursor) {
            case 0: state = MENU_STATE_LAUNCH_SNAKE; break;
            case 1: state = MENU_STATE_LAUNCH_TETRIS; break;
            case 2: state = MENU_STATE_LAUNCH_BRICK; break;
            case 3:
                EEPROM_Save_ResetAll();
                state = MENU_STATE_RESET_SCORES;
                break;
        }
    }

    if (millis() - lastBlink > 250) {
        blinkOn = !blinkOn;
        lastBlink = millis();
    }
}

void Menu_Draw(void) {
    Display_Clear();

    Display_DrawTextCenter(2, "GAME BOX", SSD1306_WHITE, 2);
    Display_DrawLine(0, 18, 127, 18, SSD1306_WHITE);

    for (int8_t i = 0; i < MENU_ITEMS; i++) {
        int16_t y = 24 + i * 10;
        if (i == cursor) {
            if (blinkOn) {
                Display_FillRect(0, y, 128, 9, SSD1306_WHITE);
                Display_DrawText(8, y + 1, menuNames[i], SSD1306_BLACK, 1);
            } else {
                Display_DrawText(8, y + 1, menuNames[i], SSD1306_WHITE, 1);
            }
        } else {
            Display_DrawText(8, y + 1, menuNames[i], SSD1306_WHITE, 1);
        }

        if (i < 3) {
            int score = EEPROM_Save_GetHighScore(i);
            char buf[8];
            itoa(score, buf, 10);
            int16_t sw = strlen(buf) * 6;
            Display_DrawText(120 - sw, y + 1, buf, (i == cursor && blinkOn) ? SSD1306_BLACK : SSD1306_WHITE, 1);
        }
    }

    Display_DrawTextCenter(62, "OK:Start", SSD1306_WHITE, 1);
    Display_Update();
}

MenuState_t Menu_GetState(void) {
    return state;
}

void Menu_SetIdle(void) {
    state = MENU_STATE_IDLE;
}
