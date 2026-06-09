#include "EEPROM_Save.h"
#include <EEPROM.h>

#define MAGIC_ADDR      0
#define MAGIC_VALUE     0xAB
#define SCORE_BASE_ADDR 1
#define GAME_COUNT      3

void EEPROM_Save_Init(void) {
    uint8_t magic = EEPROM.read(MAGIC_ADDR);
    if (magic != MAGIC_VALUE) {
        EEPROM.write(MAGIC_ADDR, MAGIC_VALUE);
        for (uint8_t i = 0; i < GAME_COUNT; i++) {
            int addr = SCORE_BASE_ADDR + i * sizeof(int);
            EEPROM.put(addr, (int)0);
        }
    }
}

int EEPROM_Save_GetHighScore(uint8_t gameIndex) {
    if (gameIndex >= GAME_COUNT) return 0;
    int val;
    EEPROM.get(SCORE_BASE_ADDR + gameIndex * sizeof(int), val);
    return val;
}

void EEPROM_Save_SetHighScore(uint8_t gameIndex, int score) {
    if (gameIndex >= GAME_COUNT) return;
    int current = EEPROM_Save_GetHighScore(gameIndex);
    if (score > current) {
        EEPROM.put(SCORE_BASE_ADDR + gameIndex * sizeof(int), score);
    }
}

void EEPROM_Save_ResetAll(void) {
    for (uint8_t i = 0; i < GAME_COUNT; i++) {
        EEPROM.put(SCORE_BASE_ADDR + i * sizeof(int), (int)0);
    }
}
