#include "Input.h"
#include "PinDefines.h"

static InputDir_t lastDir = DIR_NONE;
static bool okPressed = false;
static unsigned long lastDirTime = 0;
static unsigned long lastOkTime = 0;
static const unsigned long DIR_DELAY = 120;
static const unsigned long OK_DELAY  = 200;

void Input_Init(void) {
    pinMode(PIN_JOY_SW, INPUT_PULLUP);
    lastDir = DIR_NONE;
    okPressed = false;
    lastDirTime = 0;
    lastOkTime = 0;
}

void Input_Update(void) {
    int x = analogRead(PIN_JOY_VRX);
    int y = analogRead(PIN_JOY_VRY);
    unsigned long now = millis();

    InputDir_t newDir = DIR_NONE;
    if (x < JOY_THRESHOLD_LOW)       newDir = DIR_LEFT;
    else if (x > JOY_THRESHOLD_HIGH) newDir = DIR_RIGHT;
    else if (y < JOY_THRESHOLD_LOW)  newDir = DIR_UP;
    else if (y > JOY_THRESHOLD_HIGH) newDir = DIR_DOWN;

    if (newDir != DIR_NONE && (now - lastDirTime >= DIR_DELAY)) {
        lastDir = newDir;
        lastDirTime = now;
    } else if (newDir == DIR_NONE) {
        lastDir = DIR_NONE;
    }

    bool sw = (digitalRead(PIN_JOY_SW) == LOW);
    if (sw && (now - lastOkTime >= OK_DELAY)) {
        okPressed = true;
        lastOkTime = now;
    } else {
        okPressed = false;
    }
}

InputDir_t Input_GetDirection(void) {
    return lastDir;
}

bool Input_IsPressed(void) {
    return okPressed;
}

InputDir_t Input_GetLastDirection(void) {
    return lastDir;
}

bool Input_GetOkPressed(void) {
    return okPressed;
}
