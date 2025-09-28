#include "raylib.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

bool CPU_CAN_MOVE = true;

typedef struct Entity {
    Vector2 position;
    Vector2 size;
    float radius;
    Vector2 velocity;
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
    if (entity->position.y + entity->size.y >= (float) SCREEN_HEIGHT) {
        entity->position.y = (float) SCREEN_HEIGHT - entity->size.y;
    }
}

void UpdatePlayer(Entity *paddle) {
    if (IsKeyDown(KEY_UP)) {
        paddle->position.y = paddle->position.y - paddle->velocity.x;
    }
    if (IsKeyDown(KEY_DOWN)) {
        paddle->position.y = paddle->position.y + paddle->velocity.x;
    }

    ConstrainMovement(paddle);
}

void UpdateCpu(Entity *paddle, Entity *ball) {
    if (CPU_CAN_MOVE) {
        if (paddle->position.y + paddle->size.y * 0.5 > ball->position.y) {
            paddle->position.y = paddle->position.y - paddle->velocity.x;
        }
        if (paddle->position.y + paddle->size.y * 0.5 <= ball->position.y) {
            paddle->position.y = paddle->position.y + paddle->velocity.x;
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

void UpdateBall(Entity *ball) {
    ball->position.x += ball->velocity.x;
    ball->position.y += ball->velocity.y;

    if (ball->position.x + ball->radius >= (float) SCREEN_WIDTH || ball->position.x - ball->radius <= 0) {
        ball->velocity.x *= -1;
    }

    if (ball->position.y + ball->radius >= (float) SCREEN_HEIGHT || ball->position.y - ball->radius <= 0) {
        ball->velocity.y *= -1;
    }

    if (ball->position.x + ball->radius >= (float) SCREEN_WIDTH) {
        ResetBall(ball);
    }

    if (ball->position.x - ball->radius <= 0) {
        ResetBall(ball);
    }
}


void CollisionCheck(Entity *player, Entity *cpu, Entity *ball) {
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
        .position = {(float) SCREEN_WIDTH - 50, (float) SCREEN_HEIGHT * 0.5f},
        .size = {25, 125},
        .velocity = {6.0f, 0},
        .color = RED
    };

    Entity Cpu = (Entity){
        .position = {50, (float) SCREEN_HEIGHT * 0.5f},
        .size = {25, 125},
        .velocity = {6.0f, 0},
        .color = BLUE
    };

    Cpu.position.y -= Cpu.size.y * 0.5f;

    Entity Ball = (Entity){
        .position = {(float) SCREEN_WIDTH * 0.5f, (float) SCREEN_HEIGHT * 0.5f},
        .size = {25, 25},
        .radius = 15,
        .velocity = {7.0f, 7.0f},
        .color = ORANGE
    };


    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        UpdatePlayer(&Player);
        UpdateCpu(&Cpu, &Ball);
        UpdateBall(&Ball);

        CollisionCheck(&Player, &Cpu, &Ball);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(DARKGRAY);

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
