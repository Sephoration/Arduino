#include "Tetris.h"
#include "Display.h"
#include "Input.h"
#include "Sound.h"
#include "PinDefines.h"

#define TETRIS_BOARD_W      10
#define TETRIS_BOARD_H      16
#define TETRIS_CELL_SIZE    4
#define TETRIS_TICK_START   700
#define TETRIS_TICK_MIN     150
#define TETRIS_TICK_DROP    60

// 7 种方块 x 4 种旋转，每行 4bit 共 4 行 -> uint16_t
static const uint16_t TETROMINOS[7][4] PROGMEM = {
    {0x0F00, 0x2222, 0x00F0, 0x4444}, // I
    {0x0660, 0x0660, 0x0660, 0x0660}, // O
    {0x4700, 0x4640, 0x0740, 0x2620}, // T
    {0x6C00, 0x4620, 0x06C0, 0x2640}, // S
    {0xC600, 0x2640, 0x0C60, 0x4620}, // Z
    {0x8E00, 0x6440, 0x0E20, 0x44C0}, // J
    {0x2E00, 0x4460, 0x0E80, 0xC440}  // L
};

static uint8_t board[TETRIS_BOARD_W][TETRIS_BOARD_H];
static uint8_t curPiece;
static uint8_t curRot;
static int8_t  curX;
static int8_t  curY;
static uint8_t nextPiece;
static int16_t score;
static int16_t lines;
static bool    gameOver;
static bool    paused;
static unsigned long lastTick;
static unsigned long tickInterval;
static bool    dropFast;

static bool Tetris_GetCell(uint8_t piece, uint8_t rot, int8_t x, int8_t y) {
    if (x < 0 || x >= 4 || y < 0 || y >= 4) return false;
    uint16_t data = pgm_read_word(&TETROMINOS[piece][rot]);
    uint8_t shift = 12 - y * 4;
    uint8_t row = (data >> shift) & 0x0F;
    return (row >> (3 - x)) & 0x01;
}

static bool Tetris_CheckCollision(uint8_t piece, uint8_t rot, int8_t x, int8_t y) {
    for (int8_t py = 0; py < 4; py++) {
        for (int8_t px = 0; px < 4; px++) {
            if (Tetris_GetCell(piece, rot, px, py)) {
                int8_t bx = x + px;
                int8_t by = y + py;
                if (bx < 0 || bx >= TETRIS_BOARD_W || by >= TETRIS_BOARD_H) return true;
                if (by >= 0 && board[bx][by]) return true;
            }
        }
    }
    return false;
}

static void Tetris_MergePiece(void) {
    for (int8_t py = 0; py < 4; py++) {
        for (int8_t px = 0; px < 4; px++) {
            if (Tetris_GetCell(curPiece, curRot, px, py)) {
                int8_t bx = curX + px;
                int8_t by = curY + py;
                if (bx >= 0 && bx < TETRIS_BOARD_W && by >= 0 && by < TETRIS_BOARD_H) {
                    board[bx][by] = 1;
                }
            }
        }
    }
}

static void Tetris_ClearLines(void) {
    int8_t cleared = 0;
    for (int8_t y = TETRIS_BOARD_H - 1; y >= 0; y--) {
        bool full = true;
        for (int8_t x = 0; x < TETRIS_BOARD_W; x++) {
            if (!board[x][y]) { full = false; break; }
        }
        if (full) {
            cleared++;
            for (int8_t yy = y; yy > 0; yy--) {
                for (int8_t x = 0; x < TETRIS_BOARD_W; x++) {
                    board[x][yy] = board[x][yy - 1];
                }
            }
            for (int8_t x = 0; x < TETRIS_BOARD_W; x++) board[x][0] = 0;
            y++;
        }
    }
    if (cleared > 0) {
        lines += cleared;
        // 消行分数：1行40，2行100，3行300，4行800
        const int16_t pts[5] = {0, 40, 100, 300, 800};
        if (cleared <= 4) score += pts[cleared];
        else score += cleared * 200;
        Sound_PlayClear();
        if (tickInterval > TETRIS_TICK_MIN) tickInterval -= 15;
    }
}

static void Tetris_SpawnPiece(void) {
    curPiece = nextPiece;
    nextPiece = random(0, 7);
    curRot = 0;
    curX = TETRIS_BOARD_W / 2 - 2;
    curY = -1;
    if (Tetris_CheckCollision(curPiece, curRot, curX, curY)) {
        gameOver = true;
        Sound_PlayDie();
    }
}

void Tetris_Init(void) {
    randomSeed(analogRead(A3));
    Tetris_Reset();
}

void Tetris_Reset(void) {
    memset(board, 0, sizeof(board));
    score = 0;
    lines = 0;
    gameOver = false;
    paused = false;
    dropFast = false;
    tickInterval = TETRIS_TICK_START;
    lastTick = millis();
    nextPiece = random(0, 7);
    Tetris_SpawnPiece();
}

void Tetris_Update(void) {
    if (gameOver) return;

    InputDir_t dirEvt = Input_GetDirection();       // 事件方向（旋转用）
    InputDir_t dirHeld = Input_GetLastDirection();   // 按住方向（移动/下落用）
    bool ok = Input_GetOkPressed();

    if (ok) {
        paused = !paused;
        if (paused) Sound_PlaySelect();
        return;
    }
    if (paused) return;

    // 左右移动（支持长按，但限制速率）
    static unsigned long lastMove = 0;
    if (dirHeld == DIR_LEFT && millis() - lastMove > 100) {
        if (!Tetris_CheckCollision(curPiece, curRot, curX - 1, curY)) {
            curX--;
            lastMove = millis();
            Sound_PlayMove();
        }
    } else if (dirHeld == DIR_RIGHT && millis() - lastMove > 100) {
        if (!Tetris_CheckCollision(curPiece, curRot, curX + 1, curY)) {
            curX++;
            lastMove = millis();
            Sound_PlayMove();
        }
    }

    // 旋转（上键，边沿触发）
    static bool rotTriggered = false;
    if (dirEvt == DIR_UP) {
        if (!rotTriggered) {
            rotTriggered = true;
            uint8_t newRot = (curRot + 1) & 3;
            if (!Tetris_CheckCollision(curPiece, newRot, curX, curY)) {
                curRot = newRot;
                Sound_PlayMove();
            } else if (!Tetris_CheckCollision(curPiece, newRot, curX - 1, curY)) {
                curRot = newRot; curX--;
                Sound_PlayMove();
            } else if (!Tetris_CheckCollision(curPiece, newRot, curX + 1, curY)) {
                curRot = newRot; curX++;
                Sound_PlayMove();
            }
        }
    } else {
        rotTriggered = false;
    }

    // 快速下落（下键，按住时加速）
    dropFast = (dirHeld == DIR_DOWN);

    unsigned long now = millis();
    unsigned long interval = dropFast ? TETRIS_TICK_DROP : tickInterval;
    if (now - lastTick < interval) return;
    lastTick = now;

    // 下落一格
    if (!Tetris_CheckCollision(curPiece, curRot, curX, curY + 1)) {
        curY++;
    } else {
        Tetris_MergePiece();
        Sound_PlayBounce();
        Tetris_ClearLines();
        Tetris_SpawnPiece();
    }
}

void Tetris_Draw(void) {
    Display_Clear();

    // 画已有方块
    for (int8_t x = 0; x < TETRIS_BOARD_W; x++) {
        for (int8_t y = 0; y < TETRIS_BOARD_H; y++) {
            if (board[x][y]) {
                Display_FillRect(x * TETRIS_CELL_SIZE, y * TETRIS_CELL_SIZE,
                                 TETRIS_CELL_SIZE, TETRIS_CELL_SIZE, SSD1306_WHITE);
            }
        }
    }

    // 画当前下落块（虚影/实体）
    for (int8_t py = 0; py < 4; py++) {
        for (int8_t px = 0; px < 4; px++) {
            if (Tetris_GetCell(curPiece, curRot, px, py)) {
                int16_t sx = (curX + px) * TETRIS_CELL_SIZE;
                int16_t sy = (curY + py) * TETRIS_CELL_SIZE;
                Display_DrawRect(sx, sy, TETRIS_CELL_SIZE, TETRIS_CELL_SIZE, SSD1306_WHITE);
                Display_DrawPixel(sx + 1, sy + 1, SSD1306_WHITE);
            }
        }
    }

    // 右侧信息栏（从 x=48 开始）
    Display_DrawLine(42, 0, 42, 63, SSD1306_WHITE);

    Display_DrawText(46, 0, "NEXT", SSD1306_WHITE, 1);
    for (int8_t py = 0; py < 4; py++) {
        for (int8_t px = 0; px < 4; px++) {
            if (Tetris_GetCell(nextPiece, 0, px, py)) {
                Display_FillRect(46 + px * TETRIS_CELL_SIZE, 10 + py * TETRIS_CELL_SIZE,
                                 TETRIS_CELL_SIZE, TETRIS_CELL_SIZE, SSD1306_WHITE);
            }
        }
    }

    char buf[16];
    snprintf(buf, sizeof(buf), "L:%d", lines);
    Display_DrawText(46, 30, buf, SSD1306_WHITE, 1);
    snprintf(buf, sizeof(buf), "S:%d", score);
    Display_DrawText(46, 42, buf, SSD1306_WHITE, 1);

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

bool Tetris_IsGameOver(void) {
    return gameOver;
}

int Tetris_GetScore(void) {
    return score;
}
