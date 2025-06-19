/*
** EPITECH PROJECT, 2025
** ZappyGUI
** File description:
** Map.hpp
*/

#pragma once

#include <array>
#include <cstddef>
#include <vector>
#include <map>
#include <unordered_map>
#include "../resource/IResource.hpp"
#include "../../include/raylib-cpp.hpp"
#include "../core/Core.hpp"

namespace GUI {
    struct TileInfo;
    struct PlayerInfo;
    struct EggInfo;

    class Map {
        private:
            std::vector<std::vector<std::vector<IResource>>> _grid;
            float _tileSize;
            std::size_t _width;
            std::size_t _height;
            
            std::map<std::pair<int, int>, TileInfo> _tileData;
            std::unordered_map<std::string, PlayerInfo> _playerData;
            std::unordered_map<std::string, EggInfo> _eggData;

        public:
            std::unique_ptr<raylib::Model> _eggModel;

            Map(std::size_t width, std::size_t height, float tileSize = 1.0f);

            std::size_t get_width() const { return _width; }
            std::size_t get_height() const { return _height; }

            void drawGround();
            void drawResources();
            void drawEggs();
            void drawPlayers();

            // Méthodes pour mettre à jour les données
            void updateTileData(const std::map<std::pair<int, int>, TileInfo>& tiles);
            void updatePlayerData(const std::unordered_map<std::string, PlayerInfo>& players);
            void updateEggData(const std::unordered_map<std::string, EggInfo>& eggs);

            void render();
    };
}