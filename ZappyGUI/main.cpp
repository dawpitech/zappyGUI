#include "raylib.h"
#include <cmath>

int main() {
    InitWindow(800, 600, "Raylib 3D - Ile en voxels");
    DisableCursor();
    Camera3D camera = { 0 };
    camera.position = { 10.0f, 10.0f, 10.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);

    const int gridSize = 20;

    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_FREE);

        BeginDrawing();
        ClearBackground(BLUE);

        BeginMode3D(camera);

        for (int z = -gridSize; z <= gridSize; z++) {
            for (int x = -gridSize; x <= gridSize; x++) {
                float dist = sqrtf((float)(x * x + z * z));
                float maxDist = gridSize;
                float height = 1.5f * (1.0f - dist / maxDist); // plus haut au centre

                if (height > 0.0f) {
                    DrawCube((Vector3){ (float)x, height / 2.0f, (float)z }, 1.0f, height, 1.0f, YELLOW);
                    DrawCubeWires((Vector3){ (float)x, height / 2.0f, (float)z }, 1.0f, height, 1.0f, DARKPURPLE);
                }
            }
        }

        EndMode3D();

        DrawText("Ile en 3D avec des cubes", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
