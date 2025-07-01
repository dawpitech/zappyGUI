/*
** EPITECH PROJECT, 2025
** ZappyGUI
** File description:
** map.cpp
*/

#include <raylib.h>
#include "Map.hpp"
#include <iostream>

GUI::Map::Map(std::size_t width, std::size_t height, float tileSize)
    : _width(width), _height(height), _tileSize(tileSize)
{
    _grid.resize(width);
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

        DrawCylinder(playerPos, 0.1f, 0.1f, 0.4f, 8, playerColor);

        Vector3 arrowEnd = playerPos;
        switch (player.getorientation()) {
            case 1: arrowEnd.z -= 0.3f; break; // North
            case 2: arrowEnd.x += 0.3f; break; // East
            case 3: arrowEnd.z += 0.3f; break; // South
            case 4: arrowEnd.x -= 0.3f; break; // West
        }
        arrowEnd.y += 0.2f;

        DrawLine3D(Vector3{playerPos.x, playerPos.y + 0.2f, playerPos.z}, arrowEnd, BLACK);

        DrawSphere({playerPos.x, playerPos.y + 0.5f, playerPos.z}, 0.05f * player.getLevel(), GOLD);
    }
}

void GUI::Map::updateTileData(const std::map<std::pair<int, int>, TileInfo>& tiles)
{
    _tileData = tiles;
}

void GUI::Map::updatePlayerData(const std::unordered_map<std::string, Player>& players)
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