/*
** EPITECH PROJECT, 2025
** ZappyGUI
** File description:
** map.cpp
*/

#include "Map.hpp"

GUI::Map::Map(std::size_t width, std::size_t height, float tileSize)
    : _width(width), _height(height), _tileSize(tileSize)
{
    _grid.resize(width);
    _eggModel = std::make_unique<raylib::Model>("assets/egg.glb");
    for (auto &column : _grid)
        column.resize(height);
}

void GUI::Map::drawGround()
{
    for (std::size_t x = 0; x < _width; ++x) {
        for (std::size_t y = 0; y < _height; ++y) {
            Vector3 position = { static_cast<float>(x) * _tileSize, 0.0f, static_cast<float>(y) * _tileSize };
            DrawCube(position, _tileSize, 0.1f, _tileSize, GRAY);
            DrawCubeWires(position, _tileSize, 0.1f, _tileSize, DARKGRAY);
        }
    }
}

void GUI::Map::drawResources()
{
    for (const auto& [pos, tile] : _tileData) {
        int x = pos.first;
        int y = pos.second;
        
        Vector3 basePos = { static_cast<float>(x) * _tileSize, 0.1f, static_cast<float>(y) * _tileSize };
        
        const Color resourceColors[] = {
            YELLOW,    // food
            BLUE,      // linemate
            GREEN,     // deraumere
            RED,       // sibur
            PURPLE,    // mendiane
            ORANGE,    // phiras
            PINK       // thystame
        };
        
        for (size_t i = 0; i < tile.resources.size() && i < 7; ++i) {
            int resourceCount = tile.resources[i];
            if (resourceCount > 0) {
                Vector3 resourcePos = {
                    basePos.x + (i % 3 - 1) * 0.2f,
                    basePos.y + 0.1f,
                    basePos.z + (i / 3 - 1) * 0.2f
                };
                
                float size = 0.05f + (resourceCount * 0.02f);
                if (size > 0.2f) size = 0.2f;
                
                DrawSphere(resourcePos, size, resourceColors[i]);
            }
        }
    }
}

void GUI::Map::drawEggs()
{
    for (const auto& [id, egg] : _eggData) {
        Vector3 eggPos = {
            static_cast<float>(egg.x) * _tileSize,
            0.0f,
            static_cast<float>(egg.y) * _tileSize
        };

        _eggModel->Draw(eggPos, 0.005f, WHITE);

    }
}

void GUI::Map::drawPlayers()
{
    const Color teamColors[] = {
        RED, BLUE, GREEN, YELLOW, PURPLE, ORANGE, PINK, BROWN
    };
    
    int teamIndex = 0;
    std::map<std::string, int> teamColorMap;
    
    for (const auto& [id, player] : _playerData) {
        if (teamColorMap.find(player.team) == teamColorMap.end()) {
            teamColorMap[player.team] = teamIndex % 8;
            teamIndex++;
        }
        
        Vector3 playerPos = { 
            static_cast<float>(player.x) * _tileSize, 
            0.3f, 
            static_cast<float>(player.y) * _tileSize 
        };
        
        Color playerColor = teamColors[teamColorMap[player.team]];
        
        DrawCylinder(playerPos, 0.1f, 0.1f, 0.4f, 8, playerColor);
        
        Vector3 arrowEnd = playerPos;
        switch (player.orientation) {
            case 1: arrowEnd.z -= 0.3f; break; // North
            case 2: arrowEnd.x += 0.3f; break; // East  
            case 3: arrowEnd.z += 0.3f; break; // South
            case 4: arrowEnd.x -= 0.3f; break; // West
        }
        arrowEnd.y += 0.2f;
        
        DrawLine3D(Vector3{playerPos.x, playerPos.y + 0.2f, playerPos.z}, arrowEnd, BLACK);
        
        DrawSphere({playerPos.x, playerPos.y + 0.5f, playerPos.z}, 0.05f * player.level, GOLD);
    }
}

void GUI::Map::updateTileData(const std::map<std::pair<int, int>, TileInfo>& tiles)
{
    _tileData = tiles;
}

void GUI::Map::updatePlayerData(const std::unordered_map<std::string, PlayerInfo>& players)
{
    _playerData = players;
}

void GUI::Map::updateEggData(const std::unordered_map<std::string, EggInfo>& eggs)
{
    _eggData = eggs;
}

void GUI::Map::render()
{
    drawGround();
    drawResources();
    drawEggs();
    drawPlayers();
}