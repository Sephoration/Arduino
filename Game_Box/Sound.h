#ifndef SOUND_H
#define SOUND_H

#include <Arduino.h>

void Sound_Init(void);
void Sound_PlayTone(uint16_t freq, uint16_t duration);
void Sound_PlayMove(void);
void Sound_PlayEat(void);
void Sound_PlayClear(void);
void Sound_PlayDie(void);
void Sound_PlayBounce(void);
void Sound_PlaySelect(void);
void Sound_PlayStart(void);

#endif
