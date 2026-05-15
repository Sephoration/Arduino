#ifndef SNAKE_H
#define SNAKE_H

#include <Arduino.h>

void Snake_Init(void);
void Snake_Reset(void);
void Snake_Update(void);
void Snake_Draw(void);
bool Snake_IsGameOver(void);
int  Snake_GetScore(void);

#endif
