#include "Brick.h"
#include "Display.h"
#include "Input.h"
#include "Sound.h"
#include "PinDefines.h"

#define BRICK_COLS          8
#define BRICK_ROWS          5
#define BRICK_WIDTH         14
#define BRICK_HEIGHT        5
#define BRICK_GAP_X         2
#define BRICK_GAP_Y         2
#define BRICK_OFFSET_X      1
#define BRICK_OFFSET_Y      2
#define PADDLE_WIDTH        24
#define PADDLE_HEIGHT       4
#define PADDLE_Y            58
#define BALL_RADIUS         2
#define BALL_SIZE           (BALL_RADIUS * 2)
#define BALL_SPEED_START    1.2f
#define BALL_SPEED_MAX      2.4f
#define PADDLE_SPEED        3
#define MAX_LIVES           3

typedef struct {
    float x, y;
    float vx, vy;
} Ball_t;

static bool bricks[BRICK_ROWS][BRICK_COLS];
static int8_t lives;
static int16_t score;
static bool gameOver;
static bool win;
static bool paused;
static float paddleX;
static Ball_t ball;

static void Brick_ResetBall(void) {
    ball.x = SCREEN_WIDTH / 2.0f;
    ball.y = SCREEN_HEIGHT / 2.0f;
    ball.vx = (random(0, 2) == 0 ? 1.0f : -1.0f) * 0.8f;
    ball.vy = 1.0f;
    float speed = BALL_SPEED_START;
    float len = sqrt(ball.vx * ball.vx + ball.vy * ball.vy);
    ball.vx = (ball.vx / len) * speed;
    ball.vy = (ball.vy / len) * speed;
}

static void Brick_InitLevel(void) {
    for (int8_t r = 0; r < BRICK_ROWS; r++) {
        for (int8_t c = 0; c < BRICK_COLS; c++) {
            bricks[r][c] = true;
        }
    }
}

void Brick_Init(void) {
    randomSeed(analogRead(A3));
    Brick_Reset();
}

void Brick_Reset(void) {
    Brick_InitLevel();
    lives = MAX_LIVES;
    score = 0;
    gameOver = false;
    win = false;
    paused = false;
    paddleX = (SCREEN_WIDTH - PADDLE_WIDTH) / 2.0f;
    Brick_ResetBall();
}

static bool Brick_BallRectCollision(float cx, float cy, float rx, float ry, float rw, float rh) {
    float closestX = constrain(cx, rx, rx + rw);
    float closestY = constrain(cy, ry, ry + rh);
    float dx = cx - closestX;
    float dy = cy - closestY;
    return (dx * dx + dy * dy) < (BALL_RADIUS * BALL_RADIUS);
}

static void Brick_ReflectOffRect(float rx, float ry, float rw, float rh) {
    float closestX = constrain(ball.x, rx, rx + rw);
    float closestY = constrain(ball.y, ry, ry + rh);
    float dx = ball.x - closestX;
    float dy = ball.y - closestY;

    if (fabs(dx) > fabs(dy)) {
        ball.vx = -ball.vx;
        if (dx > 0) ball.x = rx + rw + BALL_RADIUS + 0.5f;
        else        ball.x = rx - BALL_RADIUS - 0.5f;
    } else {
        ball.vy = -ball.vy;
        if (dy > 0) ball.y = ry + rh + BALL_RADIUS + 0.5f;
        else        ball.y = ry - BALL_RADIUS - 0.5f;
    }
}

void Brick_Update(void) {
    if (gameOver || win) return;

    InputDir_t dir = Input_GetDirection();
    bool ok = Input_GetOkPressed();

    if (ok) {
        paused = !paused;
        if (paused) Sound_PlaySelect();
        return;
    }
    if (paused) return;

    // 挡板移动
    if (dir == DIR_LEFT) {
        paddleX -= PADDLE_SPEED;
        if (paddleX < 0) paddleX = 0;
    } else if (dir == DIR_RIGHT) {
        paddleX += PADDLE_SPEED;
        if (paddleX > SCREEN_WIDTH - PADDLE_WIDTH) paddleX = SCREEN_WIDTH - PADDLE_WIDTH;
    }

    // 球移动
    ball.x += ball.vx;
    ball.y += ball.vy;

    // 墙壁碰撞
    if (ball.x - BALL_RADIUS < 0) {
        ball.x = BALL_RADIUS;
        ball.vx = -ball.vx;
        Sound_PlayBounce();
    } else if (ball.x + BALL_RADIUS >= SCREEN_WIDTH) {
        ball.x = SCREEN_WIDTH - BALL_RADIUS - 1;
        ball.vx = -ball.vx;
        Sound_PlayBounce();
    }
    if (ball.y - BALL_RADIUS < 0) {
        ball.y = BALL_RADIUS;
        ball.vy = -ball.vy;
        Sound_PlayBounce();
    } else if (ball.y + BALL_RADIUS >= SCREEN_HEIGHT) {
        // 掉下去
        lives--;
        Sound_PlayDie();
        if (lives <= 0) {
            gameOver = true;
        } else {
            paddleX = (SCREEN_WIDTH - PADDLE_WIDTH) / 2.0f;
            Brick_ResetBall();
        }
        return;
    }

    // 挡板碰撞
    if (ball.vy > 0 && Brick_BallRectCollision(ball.x, ball.y, paddleX, PADDLE_Y, PADDLE_WIDTH, PADDLE_HEIGHT)) {
        float hitPos = (ball.x - (paddleX + PADDLE_WIDTH / 2.0f)) / (PADDLE_WIDTH / 2.0f);
        hitPos = constrain(hitPos, -1.0f, 1.0f);
        float speed = sqrt(ball.vx * ball.vx + ball.vy * ball.vy);
        if (speed < BALL_SPEED_MAX) speed += 0.05f;
        float angle = hitPos * (PI / 3.0f); // 最大60度
        ball.vx = sin(angle) * speed;
        ball.vy = -cos(angle) * speed;
        ball.y = PADDLE_Y - BALL_RADIUS - 0.5f;
        Sound_PlayBounce();
    }

    // 砖块碰撞
    for (int8_t r = 0; r < BRICK_ROWS; r++) {
        for (int8_t c = 0; c < BRICK_COLS; c++) {
            if (!bricks[r][c]) continue;
            float bx = BRICK_OFFSET_X + c * (BRICK_WIDTH + BRICK_GAP_X);
            float by = BRICK_OFFSET_Y + r * (BRICK_HEIGHT + BRICK_GAP_Y);
            if (Brick_BallRectCollision(ball.x, ball.y, bx, by, BRICK_WIDTH, BRICK_HEIGHT)) {
                bricks[r][c] = false;
                score += 10;
                Brick_ReflectOffRect(bx, by, BRICK_WIDTH, BRICK_HEIGHT);
                Sound_PlayEat();

                // 检查是否全部消除
                bool anyLeft = false;
                for (int8_t rr = 0; rr < BRICK_ROWS && !anyLeft; rr++) {
                    for (int8_t cc = 0; cc < BRICK_COLS; cc++) {
                        if (bricks[rr][cc]) { anyLeft = true; break; }
                    }
                }
                if (!anyLeft) {
                    win = true;
                    Sound_PlayClear();
                }
                return; // 每帧只处理一块碰撞，避免穿模
            }
        }
    }
}

void Brick_Draw(void) {
    Display_Clear();

    // 砖块
    for (int8_t r = 0; r < BRICK_ROWS; r++) {
        for (int8_t c = 0; c < BRICK_COLS; c++) {
            if (bricks[r][c]) {
                int16_t x = BRICK_OFFSET_X + c * (BRICK_WIDTH + BRICK_GAP_X);
                int16_t y = BRICK_OFFSET_Y + r * (BRICK_HEIGHT + BRICK_GAP_Y);
                Display_FillRect(x, y, BRICK_WIDTH, BRICK_HEIGHT, SSD1306_WHITE);
            }
        }
    }

    // 挡板
    Display_FillRect((int16_t)paddleX, PADDLE_Y, PADDLE_WIDTH, PADDLE_HEIGHT, SSD1306_WHITE);

    // 球
    Display_FillCircle((int16_t)ball.x, (int16_t)ball.y, BALL_RADIUS, SSD1306_WHITE);

    // 生命和分数
    char buf[16];
    snprintf(buf, sizeof(buf), "L:%d S:%d", lives, score);
    Display_DrawText(0, 54, buf, SSD1306_WHITE, 1);

    if (paused) {
        Display_DrawTextCenter(30, "PAUSED", SSD1306_WHITE, 2);
    }
    if (gameOver) {
        Display_DrawTextCenter(24, "GAME OVER", SSD1306_WHITE, 2);
        snprintf(buf, sizeof(buf), "Score:%d", score);
        Display_DrawTextCenter(42, buf, SSD1306_WHITE, 1);
        Display_DrawTextCenter(54, "OK:Menu", SSD1306_WHITE, 1);
    }
    if (win) {
        Display_DrawTextCenter(24, "YOU WIN!", SSD1306_WHITE, 2);
        snprintf(buf, sizeof(buf), "Score:%d", score);
        Display_DrawTextCenter(42, buf, SSD1306_WHITE, 1);
        Display_DrawTextCenter(54, "OK:Menu", SSD1306_WHITE, 1);
    }

    Display_Update();
}

bool Brick_IsGameOver(void) {
    return gameOver;
}

bool Brick_IsWin(void) {
    return win;
}

int Brick_GetScore(void) {
    return score;
}
