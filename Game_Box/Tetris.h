#ifndef TETRIS_H
#define TETRIS_H

#include <Arduino.h>

void Tetris_Init(void);
void Tetris_Reset(void);
void Tetris_Update(void);
void Tetris_Draw(void);
bool Tetris_IsGameOver(void);
int  Tetris_GetScore(void);

#endif
