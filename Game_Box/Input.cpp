#include "Input.h"
#include "PinDefines.h"

static InputDir_t heldDir    = DIR_NONE;   // 当前按住的摇杆方向
static InputDir_t dirEvent   = DIR_NONE;   // 方向变化事件（从回中进入方向时触发一次）
static InputDir_t prevHeld   = DIR_NONE;
static bool        okPressed = false;      // OK按下事件
static bool        okHeld    = false;
static bool        okPrev    = false;
static unsigned long lastOkEdge = 0;

void Input_Init(void) {
    pinMode(PIN_JOY_SW, INPUT_PULLUP);
    heldDir    = DIR_NONE;
    dirEvent   = DIR_NONE;
    prevHeld   = DIR_NONE;
    okPressed  = false;
    okHeld     = false;
    okPrev     = false;
    lastOkEdge = 0;
}

void Input_Update(void) {
    int x = analogRead(PIN_JOY_VRX);
    int y = analogRead(PIN_JOY_VRY);
    unsigned long now = millis();

    // 1. 摇杆方向
    InputDir_t newDir = DIR_NONE;
    if (x < JOY_THRESHOLD_LOW)       newDir = DIR_LEFT;
    else if (x > JOY_THRESHOLD_HIGH) newDir = DIR_RIGHT;
    else if (y < JOY_THRESHOLD_LOW)  newDir = DIR_UP;
    else if (y > JOY_THRESHOLD_HIGH) newDir = DIR_DOWN;

    heldDir = newDir;                         // 实时方向，游戏里面用
    dirEvent = DIR_NONE;                      // 默认为无事件
    if (newDir != DIR_NONE && prevHeld == DIR_NONE) {
        dirEvent = newDir;                    // 从回中进入方向，触发一次事件
    }
    prevHeld = newDir;

    // 2. OK键（下降沿触发）
    bool sw = (digitalRead(PIN_JOY_SW) == LOW);
    okHeld = sw;
    okPressed = false;
    if (sw && !okPrev && (now - lastOkEdge > 100)) {
        okPressed = true;
        lastOkEdge = now;
    }
    okPrev = sw;
}

InputDir_t Input_GetDirection(void) {
    return dirEvent;            // 菜单/选项用事件式方向
}

bool Input_IsPressed(void) {
    return okPressed;
}

InputDir_t Input_GetLastDirection(void) {
    return heldDir;             // 游戏用按住时的方向
}

bool Input_GetOkPressed(void) {
    return okPressed;
}
