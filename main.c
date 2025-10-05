#include "raylib.h"

const float SCREEN_WIDTH = 800.0f;
const float SCREEN_HEIGHT = 600.0f;

const float targetFrameRate = 60.0f;

bool CPU_CAN_MOVE = true;

unsigned int player_score = 0;
unsigned int cpu_score = 0;

typedef struct Entity {
    Vector2 position;
    Vector2 size;
    float radius;

    union {
        Vector2 velocity;
        float fixedMovementSpeed;
    };

    Color color;
} Entity;

void DrawBall(const Entity *ball) {
    DrawCircle((int) ball->position.x, (int) ball->position.y, ball->radius, ball->color);
}

void DrawPaddle(const Entity *paddle) {
    DrawRectangleV(paddle->position, paddle->size, paddle->color);
}

void ConstrainMovement(Entity *entity) {
    if (entity->position.y <= 0) {
        entity->position.y = 0;
    }
    if (entity->position.y + entity->size.y >= SCREEN_HEIGHT) {
        entity->position.y = SCREEN_HEIGHT - entity->size.y;
    }
}

void UpdatePlayer(Entity *paddle, const float timeScale) {
    if (IsKeyDown(KEY_UP)) {
        paddle->position.y = paddle->position.y - paddle->fixedMovementSpeed * timeScale;
    }
    if (IsKeyDown(KEY_DOWN)) {
        paddle->position.y = paddle->position.y + paddle->fixedMovementSpeed * timeScale;
    }

    ConstrainMovement(paddle);
}

void UpdateCpu(Entity *paddle, const Entity *ball, const float timeScale) {
    if (CPU_CAN_MOVE) {
        if (paddle->position.y + paddle->size.y * 0.5 > ball->position.y) {
            paddle->position.y = paddle->position.y - paddle->fixedMovementSpeed * timeScale;
        }
        if (paddle->position.y + paddle->size.y * 0.5 <= ball->position.y) {
            paddle->position.y = paddle->position.y + paddle->fixedMovementSpeed * timeScale;
        }
    }
    ConstrainMovement(paddle);
}

void ResetBall(Entity *ball) {
    ball->position.x = SCREEN_WIDTH * 0.5;
    ball->position.y = SCREEN_HEIGHT * 0.5;

    int speed_choices[2] = {-7, 7};

    ball->velocity.x = speed_choices[GetRandomValue(0, 1)];
    ball->velocity.y = speed_choices[GetRandomValue(0, 1)];

    CPU_CAN_MOVE = true;
}

void UpdateBall(Entity *ball, float timeScale) {
    ball->position.x += ball->velocity.x * timeScale;
    ball->position.y += ball->velocity.y * timeScale;

    if (ball->position.x + ball->radius >= SCREEN_WIDTH || ball->position.x - ball->radius <= 0) {
        ball->velocity.x *= -1;
    }

    if (ball->position.y + ball->radius >= SCREEN_HEIGHT || ball->position.y - ball->radius <= 0) {
        ball->velocity.y *= -1;
    }

    if (ball->position.x + ball->radius >= SCREEN_WIDTH) {
        cpu_score++;
        ResetBall(ball);
    }

    if (ball->position.x - ball->radius <= 0) {
        player_score++;
        ResetBall(ball);
    }
}


void CollisionCheck(const Entity *player, const Entity *cpu, Entity *ball) {
    if (CheckCollisionCircleRec(ball->position, ball->radius, (Rectangle){
                                    player->position.x, player->position.y, player->size.x, player->size.y
                                })) {
        ball->velocity.x *= -1;
        CPU_CAN_MOVE = true;
    }
    if (CheckCollisionCircleRec(ball->position, ball->radius,
                                (Rectangle){cpu->position.x, cpu->position.y, cpu->size.x, cpu->size.y})) {
        ball->velocity.x *= -1;
        CPU_CAN_MOVE = false;
    }
}


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------


    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    Entity Player = (Entity)
    {
        .position = {SCREEN_WIDTH - 50, SCREEN_HEIGHT * 0.5f},
        .size = {25, 125},
        .fixedMovementSpeed = 6.0f,
        .color = RED
    };

    Entity Cpu = (Entity){
        .position = {50, SCREEN_HEIGHT * 0.5f},
        .size = {25, 125},
        .fixedMovementSpeed = 6.0f,
        .color = BLUE
    };

    Cpu.position.y -= Cpu.size.y * 0.5f;

    Entity Ball = (Entity){
        .position = {SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f},
        .size = {25, 25},
        .radius = 15,
        .velocity = {8.0f, 8.0f},
        .color = ORANGE
    };


    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        const float timeScale = targetFrameRate * GetFrameTime();

        // Update
        UpdatePlayer(&Player, timeScale);
        UpdateCpu(&Cpu, &Ball, timeScale);
        UpdateBall(&Ball, timeScale);

        CollisionCheck(&Player, &Cpu, &Ball);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(DARKGRAY);
        DrawRectangle(SCREEN_WIDTH * 0.5, 0, 5, SCREEN_HEIGHT, LIGHTGRAY);

        DrawText(TextFormat("Frame Time: %f", timeScale), 20, 5, 25, WHITE);

        DrawText(TextFormat("%lu", cpu_score), SCREEN_WIDTH * 0.25f - 20, 20, 80, WHITE);
        DrawText(TextFormat("%lu", player_score), 3 * SCREEN_WIDTH * 0.25f - 20, 20, 80, WHITE);

        DrawPaddle(&Player);
        DrawPaddle(&Cpu);
        DrawBall(&Ball);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
