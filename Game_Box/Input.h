#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>

typedef enum {
    DIR_NONE = 0,
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_OK
} InputDir_t;

void Input_Init(void);
InputDir_t Input_GetDirection(void);
bool Input_IsPressed(void);
void Input_Update(void);
InputDir_t Input_GetLastDirection(void);
bool Input_GetOkPressed(void);

#endif
