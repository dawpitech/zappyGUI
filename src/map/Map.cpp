/*
** EPITECH PROJECT, 2025
** ZappyGUI
** File description:
** map.cpp
*/

#include "Map.hpp"

template <std::size_t width, std::size_t height>
GUI::Map<width, height>::Map(float tileSize) : _tileSize(tileSize) {
}

template <std::size_t width, std::size_t height>
void GUI::Map<width, height>::drawGround()
{
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            Vector3 position = { x * _tileSize, 0.0f, y * _tileSize };
            DrawCube(position, _tileSize, 0.1f, _tileSize, GRAY);
            DrawCubeWires(position, _tileSize, 0.1f, _tileSize, DARKGRAY);
        }
    }
}

template <std::size_t width, std::size_t height>
void GUI::Map<width, height>::render()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    raylib::Window window(screenWidth, screenHeight, "Zappy GUI - 3D Grid");
    raylib::Camera3D camera(
        { width / 2.0f, 20.0f, height * 1.5f }, // position
        { width / 2.0f, 0.0f, height / 2.0f },  // target
        { 0.0f, 1.0f, 0.0f },                  // up
        45.0f, CAMERA_PERSPECTIVE
    );


    while (!window.ShouldClose()) {
        camera.Update(CAMERA_ORBITAL);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        drawGround(width, height, _tileSize);
        EndMode3D();

        DrawText("Use mouse and arrows to move the camera", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }
}
