#include "Sound.h"
#include "PinDefines.h"

void Sound_Init(void) {
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);
}

void Sound_PlayTone(uint16_t freq, uint16_t duration) {
    tone(PIN_BUZZER, freq, duration);
}

void Sound_PlayMove(void) {
    tone(PIN_BUZZER, 200, 30);
}

void Sound_PlayEat(void) {
    tone(PIN_BUZZER, 800, 60);
    delay(20);
    tone(PIN_BUZZER, 1200, 80);
}

void Sound_PlayClear(void) {
    tone(PIN_BUZZER, 600, 80);
    delay(40);
    tone(PIN_BUZZER, 800, 80);
    delay(40);
    tone(PIN_BUZZER, 1000, 120);
}

void Sound_PlayDie(void) {
    tone(PIN_BUZZER, 400, 150);
    delay(100);
    tone(PIN_BUZZER, 300, 150);
    delay(100);
    tone(PIN_BUZZER, 200, 300);
}

void Sound_PlayBounce(void) {
    tone(PIN_BUZZER, 500, 40);
}

void Sound_PlaySelect(void) {
    tone(PIN_BUZZER, 700, 50);
}

void Sound_PlayStart(void) {
    tone(PIN_BUZZER, 500, 100);
    delay(60);
    tone(PIN_BUZZER, 700, 100);
    delay(60);
    tone(PIN_BUZZER, 900, 150);
}
