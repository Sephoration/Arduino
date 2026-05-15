#ifndef BRICK_H
#define BRICK_H

#include <Arduino.h>

void Brick_Init(void);
void Brick_Reset(void);
void Brick_Update(void);
void Brick_Draw(void);
bool Brick_IsGameOver(void);
bool Brick_IsWin(void);
int  Brick_GetScore(void);

#endif
