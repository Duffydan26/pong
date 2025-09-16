#include "raylib.h"
#include "raymath.h"

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

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------


    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    Entity Player = (Entity){ .position = {25,
        (float)SCREEN_HEIGHT * 0.5f},
        .size = {30, 60},
        .velocity = 50.0f,
        .color = RED};


    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

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