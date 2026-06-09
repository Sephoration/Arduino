#ifndef MENU_H
#define MENU_H

#include <Arduino.h>

typedef enum {
    MENU_STATE_IDLE = 0,
    MENU_STATE_LAUNCH_SNAKE,
    MENU_STATE_LAUNCH_TETRIS,
    MENU_STATE_LAUNCH_BRICK,
    MENU_STATE_RESET_SCORES
} MenuState_t;

void Menu_Init(void);
void Menu_Update(void);
void Menu_Draw(void);
MenuState_t Menu_GetState(void);
void Menu_SetIdle(void);

#endif
