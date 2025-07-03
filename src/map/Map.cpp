/**
 * @file Map.cpp
 * @brief Implementation of the Map class for the ZappyGUI project
 * @author EPITECH PROJECT, 2025
 * @date 2025
 *
 * This file contains the implementation of the Map class which handles
 * the rendering and management of the game map including ground tiles,
 * resources, eggs, players, and broadcast messages in a 3D environment.
 */

/*
** EPITECH PROJECT, 2025
** ZappyGUI
** File description:
** map.cpp
*/

#include <raylib.h>
#include "Map.hpp"
#include <iostream>

/**
 * @brief Constructs a new Map object
 *
 * Initializes the map with specified dimensions and tile size. Loads all 3D models
 * for game entities including eggs, players, resources, and food. The grid is
 * resized to match the specified width and height.
 *
 * @param width The width of the map in tiles
 * @param height The height of the map in tiles
 * @param tileSize The size of each tile in world units
 *
 * @throws std::exception If any 3D model fails to load
 */
GUI::Map::Map(std::size_t width, std::size_t height, float tileSize)
    : _width(width), _height(height), _tileSize(tileSize)
{
    _grid.resize(width);
    _eggModel = std::make_unique<raylib::Model>("assets/egg.glb");
    _playerModel = std::make_unique<raylib::Model>("assets/player.glb");
    for (auto &column : _grid)
        column.resize(height);

    try {
        _eggModel = std::make_unique<raylib::Model>("assets/egg.glb");
        _linemateModel = std::make_unique<raylib::Model>("assets/firstmineral.glb");
        _deraumereModel = std::make_unique<raylib::Model>("assets/secondmineral.glb");
        _siburModel = std::make_unique<raylib::Model>("assets/thirdmineral.glb");
        _mendianeModel = std::make_unique<raylib::Model>("assets/fourthmineral.glb");
        _phirasModel = std::make_unique<raylib::Model>("assets/fifthmineral.glb");
        _thystameModel = std::make_unique<raylib::Model>("assets/sixthmineral.glb");
        _foodModel = std::make_unique<raylib::Model>("assets/food.glb");

    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement des modèles 3D: " << e.what() << std::endl;
    }
}

/**
 * @brief Draws the ground tiles of the map
 *
 * Renders a checkered pattern of ground tiles using cubes. Each tile alternates
 * between GREEN and DARKGREEN colors to create a visual grid pattern. White
 * wireframes are drawn around each tile for better visibility.
 *
 * The tiles are positioned based on their grid coordinates multiplied by the tile size.
 * Each tile has a height of 0.1 world units.
 */
void GUI::Map::drawGround()
{
    for (std::size_t x = 0; x < _width; ++x) {
        for (std::size_t y = 0; y < _height; ++y) {
            Vector3 position = { static_cast<float>(x) * _tileSize, 0.0f, static_cast<float>(y) * _tileSize };
            DrawCube(position, _tileSize, 0.1f, _tileSize, (x + y) % 2 == 0 ? GREEN : DARKGREEN);
            DrawCubeWires(position, _tileSize, 0.1f, _tileSize, WHITE);
        }
    }
}

/**
 * @brief Draws all resources on the map
 *
 * Renders resources on each tile based on the stored tile data. Resources are
 * represented by 3D models when available, or fallback to colored spheres.
 * Multiple instances of the same resource are positioned in a grid pattern
 * on the tile with slight offsets to avoid overlap.
 *
 * The function handles up to 7 different resource types:
 * - Food (YELLOW)
 * - Linemate (BLUE)
 * - Deraumere (GREEN)
 * - Sibur (RED)
 * - Mendiane (PURPLE)
 * - Phiras (ORANGE)
 * - Thystame (PINK)
 *
 * Each resource type can have up to 5 instances displayed per tile.
 */
void GUI::Map::drawResources()
{
    raylib::Model* resourceModels[] = {
        _foodModel.get(),
        _linemateModel.get(),
        _deraumereModel.get(),
        _siburModel.get(),
        _mendianeModel.get(),
        _phirasModel.get(),
        _thystameModel.get()
    };

    const Color resourceColors[] = {
        YELLOW,
        BLUE,
        GREEN,
        RED,
        PURPLE,
        ORANGE,
        PINK
    };

    for (const auto& [pos, tile] : _tileData) {
        int x = pos.first;
        int y = pos.second;
        Vector3 basePos = { static_cast<float>(x) * _tileSize, 0.1f, static_cast<float>(y) * _tileSize };

        for (size_t i = 0; i < tile.resources.size() && i < 7; ++i) {
            int resourceCount = tile.resources[i];
            if (resourceCount > 0) {
                for (int count = 0; count < resourceCount && count < 5; ++count) {
                    Vector3 resourcePos = {
                        basePos.x + ((count % 3) - 1) * 0.15f + ((i % 2) * 0.1f),
                        basePos.y + 0.05f,
                        basePos.z + ((count / 3) - 1) * 0.15f + ((i / 2) * 0.1f)
                    };

                    if (resourceModels[i] != nullptr) {
                        try {
                            resourceModels[i]->Draw(resourcePos, 0.1f, WHITE);
                        } catch (const std::exception& e) {
                            float size = 0.05f + (resourceCount * 0.02f);
                            if (size > 0.2f) size = 0.2f;
                            DrawSphere(resourcePos, size, resourceColors[i]);
                        }
                    } else {
                        float size = 0.05f + (resourceCount * 0.02f);
                        if (size > 0.2f) size = 0.2f;
                        DrawSphere(resourcePos, size, resourceColors[i]);
                    }
                }
            }
        }
    }
}

/**
 * @brief Draws all eggs on the map
 *
 * Renders eggs at their respective positions using 3D models when available,
 * or fallback to beige spheres. Eggs are positioned at ground level (y = 0)
 * and scaled to a small size (0.005f) to maintain proper proportions.
 *
 * Each egg is identified by a unique ID and has specific x, y coordinates
 * stored in the _eggData map.
 */
void GUI::Map::drawEggs()
{
    for (const auto& [id, egg] : _eggData) {
        Vector3 eggPos = {
            static_cast<float>(egg.x) * _tileSize,
            0.0f,
            static_cast<float>(egg.y) * _tileSize
        };

        if (_eggModel) {
            _eggModel->Draw(eggPos, 0.005f, WHITE);
        } else {
            DrawSphere(eggPos, 0.1f, BEIGE);
        }
    }
}

/**
 * @brief Draws all players on the map
 *
 * Renders players with team-specific colors and proper orientation.
 * Each team is assigned a unique color from a predefined palette.
 * Players are positioned at y = 0.3f to appear above the ground.
 *
 * Player orientation is handled as follows:
 * - 1: North (180 degrees)
 * - 2: East (90 degrees)
 * - 3: South (0 degrees)
 * - 4: West (270 degrees)
 *
 * The function supports up to 8 different team colors:
 * RED, BLUE, GREEN, YELLOW, PURPLE, ORANGE, PINK, BROWN
 */
void GUI::Map::drawPlayers()
{
    const Color teamColors[] = {
        RED, BLUE, GREEN, YELLOW, PURPLE, ORANGE, PINK, BROWN
    };

    int teamIndex = 0;
    std::map<std::string, int> teamColorMap;

    for (const auto& [id, player] : _playerData) {
        if (teamColorMap.find(player.getTeam()) == teamColorMap.end()) {
            teamColorMap[player.getTeam()] = teamIndex % 8;
            teamIndex++;
        }

        Vector3 playerPos = {
            static_cast<float>(player.getX()) * _tileSize,
            0.3f,
            static_cast<float>(player.getY()) * _tileSize
        };

        Color playerColor = teamColors[teamColorMap[player.getTeam()]];

        float orientationDegree;
        switch (player.getOrientation()) {
            case 1: orientationDegree = 180.0F; break; // North
            case 2: orientationDegree = 90.0F; break; // East
            case 3: orientationDegree = 0.0F; break; // South
            case 4: orientationDegree = 270.0F; break; // West
        }
        _playerModel->Draw(playerPos, {0, 1, 0}, orientationDegree, {1.0F, 1.0F, 1.0F}, WHITE);
    }
}

/**
 * @brief Draws broadcast messages from players
 *
 * Renders speech bubbles above players who have active broadcast messages.
 * The messages are displayed as 2D UI elements that follow the player's
 * 3D position in screen space. Each bubble includes:
 * - A rounded rectangle background with transparency
 * - A triangular pointer pointing to the player
 * - The message text centered in the bubble
 *
 * Messages are automatically cleared when they expire. The bubble size
 * adapts to the message length, and the position is calculated using
 * world-to-screen conversion.
 *
 * @param camera The 3D camera used for world-to-screen conversion
 */
void GUI::Map::drawBroadcastMessages(const Camera3D& camera)
{
    for (auto& [id, player] : _playerData) {
        const_cast<Player&>(player).clearBroadcastIfExpired();

        if (player.shouldShowBroadcast()) {
            Vector3 playerPos = {
                static_cast<float>(player.getX()) * _tileSize,
                0.8f,
                static_cast<float>(player.getY()) * _tileSize
            };

            Vector2 screenPos = GetWorldToScreen(playerPos, camera);

            const std::string &message = player.getBroadcastMessage();

            int textWidth = MeasureText(message.c_str(), 16);
            int bubbleWidth = textWidth + 20;
            int bubbleHeight = 30;

            Rectangle bubble = {
                screenPos.x - bubbleWidth / 2.0f,
                screenPos.y - 40,
                static_cast<float>(bubbleWidth),
                static_cast<float>(bubbleHeight)
            };

            DrawRectangleRounded(bubble, 0.3f, 8, Fade(WHITE, 0.9f));
            DrawRectangleRoundedLines(bubble, 0.3f, 8, BLACK);

            Vector2 trianglePoints[3] = {
                {screenPos.x - 8, bubble.y + bubble.height},
                {screenPos.x + 8, bubble.y + bubble.height},
                {screenPos.x, bubble.y + bubble.height + 10}
            };

            DrawTriangle(trianglePoints[0], trianglePoints[1], trianglePoints[2], Fade(WHITE, 0.9f));
            DrawTriangleLines(trianglePoints[0], trianglePoints[1], trianglePoints[2], BLACK);

            Vector2 textPos = {
                bubble.x + (bubble.width - textWidth) / 2.0f,
                bubble.y + (bubble.height - 16) / 2.0f
            };

            DrawText(message.c_str(), static_cast<int>(textPos.x), static_cast<int>(textPos.y), 16, BLACK);
        }
    }
}

/**
 * @brief Updates the tile data for the map
 *
 * Replaces the current tile data with new information. This includes
 * resource counts and other tile-specific information that affects
 * how resources are rendered on the map.
 *
 * @param tiles A map containing tile positions as keys and TileInfo as values
 */
void GUI::Map::updateTileData(const std::map<std::pair<int, int>, TileInfo>& tiles)
{
    _tileData = tiles;
}

/**
 * @brief Updates the player data for the map
 *
 * Replaces the current player data with new information. This includes
 * player positions, orientations, teams, and other player-specific data
 * that affects how players are rendered on the map.
 *
 * @param players An unordered map containing player IDs as keys and Player objects as values
 */
void GUI::Map::updatePlayerData(const std::unordered_map<std::string, Player>& players)
{
    _playerData = players;
}

/**
 * @brief Updates the egg data for the map
 *
 * Replaces the current egg data with new information. This includes
 * egg positions and other egg-specific data that affects how eggs
 * are rendered on the map.
 *
 * @param eggs An unordered map containing egg IDs as keys and EggInfo objects as values
 */
void GUI::Map::updateEggData(const std::unordered_map<std::string, EggInfo>& eggs)
{
    _eggData = eggs;
}

/**
 * @brief Renders the main 3D elements of the map
 *
 * Performs the complete rendering sequence for all 3D elements in the correct order:
 * 1. Ground tiles (base layer)
 * 2. Resources (on top of ground)
 * 3. Eggs (game entities)
 * 4. Players (top layer for visibility)
 *
 * This method should be called during the 3D rendering phase of the game loop.
 */
void GUI::Map::render()
{
    drawGround();
    drawResources();
    drawEggs();
    drawPlayers();
}

/**
 * @brief Renders the 2D UI elements of the map
 *
 * Renders UI elements that need to be displayed in screen space rather than
 * world space. Currently handles broadcast messages that appear as speech
 * bubbles above players.
 *
 * This method should be called during the 2D rendering phase of the game loop,
 * after the 3D rendering is complete.
 *
 * @param camera The 3D camera used for world-to-screen position conversion
 */
void GUI::Map::renderUI(const Camera3D& camera)
{
    drawBroadcastMessages(camera);
}
