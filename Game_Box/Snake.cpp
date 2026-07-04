#include "Snake.h"
#include "Display.h"
#include "Input.h"
#include "Sound.h"
#include "PinDefines.h"

#define SNAKE_CELL_SIZE     4
#define SNAKE_COLS          (SCREEN_WIDTH  / SNAKE_CELL_SIZE)
#define SNAKE_ROWS          (SCREEN_HEIGHT / SNAKE_CELL_SIZE)
#define SNAKE_MAX_LEN       (SNAKE_COLS * SNAKE_ROWS)
#define SNAKE_TICK_MIN      80
#define SNAKE_TICK_START    200

static int8_t snakeX[SNAKE_MAX_LEN];
static int8_t snakeY[SNAKE_MAX_LEN];
static int16_t snakeLen;
static int16_t headIdx;
static int8_t dirX;
static int8_t dirY;
static int8_t nextDirX;
static int8_t nextDirY;
static int8_t foodX;
static int8_t foodY;
static int16_t score;
static bool gameOver;
static unsigned long lastTick;
static unsigned long tickInterval;
static bool paused;

static void Snake_SpawnFood(void) {
    bool valid;
    do {
        valid = true;
        foodX = random(0, SNAKE_COLS);
        foodY = random(0, SNAKE_ROWS);
        for (int16_t i = 0; i < snakeLen; i++) {
            int8_t idx = (headIdx + SNAKE_MAX_LEN - i) % SNAKE_MAX_LEN;
            if (snakeX[idx] == foodX && snakeY[idx] == foodY) {
                valid = false;
                break;
            }
        }
    } while (!valid);
}

static bool Snake_IsSelfCollision(int8_t x, int8_t y) {
    for (int16_t i = 1; i < snakeLen; i++) {
        int8_t idx = (headIdx + SNAKE_MAX_LEN - i) % SNAKE_MAX_LEN;
        if (snakeX[idx] == x && snakeY[idx] == y) return true;
    }
    return false;
}

void Snake_Init(void) {
    randomSeed(analogRead(A3));
    Snake_Reset();
}

void Snake_Reset(void) {
    snakeLen = 3;
    headIdx = 2;
    snakeX[0] = 5; snakeY[0] = 7;
    snakeX[1] = 6; snakeY[1] = 7;
    snakeX[2] = 7; snakeY[2] = 7;
    dirX = 1; dirY = 0;
    nextDirX = 1; nextDirY = 0;
    score = 0;
    gameOver = false;
    paused = false;
    tickInterval = SNAKE_TICK_START;
    lastTick = millis();
    Snake_SpawnFood();
}

void Snake_Update(void) {
    if (gameOver) return;

    InputDir_t in = Input_GetDirection();
    if (in == DIR_UP && dirY == 0)    { nextDirX = 0; nextDirY = -1; }
    else if (in == DIR_DOWN && dirY == 0) { nextDirX = 0; nextDirY = 1; }
    else if (in == DIR_LEFT && dirX == 0) { nextDirX = -1; nextDirY = 0; }
    else if (in == DIR_RIGHT && dirX == 0){ nextDirX = 1; nextDirY = 0; }

    if (Input_GetOkPressed()) {
        paused = !paused;
        if (paused) Sound_PlaySelect();
    }

    if (paused) return;

    unsigned long now = millis();
    if (now - lastTick < tickInterval) return;
    lastTick = now;

    dirX = nextDirX;
    dirY = nextDirY;

    int8_t newX = snakeX[headIdx] + dirX;
    int8_t newY = snakeY[headIdx] + dirY;

    if (newX < 0 || newX >= SNAKE_COLS || newY < 0 || newY >= SNAKE_ROWS) {
        gameOver = true;
        Sound_PlayDie();
        return;
    }
    if (Snake_IsSelfCollision(newX, newY)) {
        gameOver = true;
        Sound_PlayDie();
        return;
    }

    headIdx = (headIdx + 1) % SNAKE_MAX_LEN;
    snakeX[headIdx] = newX;
    snakeY[headIdx] = newY;

    if (newX == foodX && newY == foodY) {
        snakeLen++;
        score += 10;
        Sound_PlayEat();
        Snake_SpawnFood();
        if (tickInterval > SNAKE_TICK_MIN) tickInterval -= 3;
    }
}

void Snake_Draw(void) {
    Display_Clear();

    for (int16_t i = 0; i < snakeLen; i++) {
        int8_t idx = (headIdx + SNAKE_MAX_LEN - i) % SNAKE_MAX_LEN;
        int16_t px = snakeX[idx] * SNAKE_CELL_SIZE;
        int16_t py = snakeY[idx] * SNAKE_CELL_SIZE;
        if (i == 0) {
            Display_FillRect(px, py, SNAKE_CELL_SIZE, SNAKE_CELL_SIZE, SSD1306_WHITE);
        } else {
            Display_DrawRect(px, py, SNAKE_CELL_SIZE, SNAKE_CELL_SIZE, SSD1306_WHITE);
        }
    }

    Display_FillRect(foodX * SNAKE_CELL_SIZE + 1, foodY * SNAKE_CELL_SIZE + 1,
                     SNAKE_CELL_SIZE - 2, SNAKE_CELL_SIZE - 2, SSD1306_WHITE);

    char buf[16];
    snprintf(buf, sizeof(buf), "S:%d", score);
    Display_DrawText(0, 0, buf, SSD1306_WHITE, 1);

    if (paused) {
        Display_DrawTextCenter(28, "PAUSED", SSD1306_WHITE, 2);
    }
    if (gameOver) {
        Display_DrawTextCenter(24, "GAME OVER", SSD1306_WHITE, 2);
        snprintf(buf, sizeof(buf), "Score:%d", score);
        Display_DrawTextCenter(42, buf, SSD1306_WHITE, 1);
        Display_DrawTextCenter(54, "OK:Menu", SSD1306_WHITE, 1);
    }

    Display_Update();
}

bool Snake_IsGameOver(void) {
    return gameOver;
}

int Snake_GetScore(void) {
    return score;
}
