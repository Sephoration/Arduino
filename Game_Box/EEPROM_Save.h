#ifndef EEPROM_SAVE_H
#define EEPROM_SAVE_H

#include <Arduino.h>

void EEPROM_Save_Init(void);
int  EEPROM_Save_GetHighScore(uint8_t gameIndex);
void EEPROM_Save_SetHighScore(uint8_t gameIndex, int score);
void EEPROM_Save_ResetAll(void);

#endif
