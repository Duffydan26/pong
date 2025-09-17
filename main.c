#include "raylib.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

typedef struct Entity {
    Vector2 position;
    Vector2 size;
    float velocity;
    Color color;
}Entity;

void DrawPaddle(const Entity* paddle) {
    DrawRectangleV(paddle->position, paddle->size, paddle->color);
}

void ConstrainMovement(Entity* paddle) {
    if (paddle->position.y <= 0){
        paddle->position.y = 0;
    }
    if (paddle->position.y + paddle->size.y >= (float)GetScreenHeight()) {
        paddle->position.y = (float)GetScreenHeight() - paddle->size.y;
    }
}

void UpdatePlayer(Entity* paddle) {
    if (IsKeyDown(KEY_UP)) {
        paddle->position.y = paddle->position.y - paddle->velocity;
    }
    if (IsKeyDown(KEY_DOWN)) {
        paddle->position.y = paddle->position.y + paddle->velocity;
    }

    ConstrainMovement(paddle);
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------


    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    Entity Player = (Entity)
        { .position = {25,(float)SCREEN_HEIGHT * 0.5f},
        .size = {25, 125},
        .velocity = 6.0f,
        .color = RED};


    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        UpdatePlayer(&Player);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawPaddle(&Player);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}