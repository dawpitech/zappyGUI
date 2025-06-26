/*
** EPITECH PROJECT, 2025
** ZappyGUI
** File description:
** Map.hpp
*/

#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include "../core/Core.hpp"

namespace GUI {
    struct TileInfo;
    struct PlayerInfo;
    struct EggInfo;

    class Map {

        private:
            std::size_t _width;
            std::size_t _height;
            float _tileSize;
            std::vector<std::vector<int>> _grid;

            std::unique_ptr<raylib::Model> _eggModel;
            std::unique_ptr<raylib::Model> _playerModel;
            std::unique_ptr<raylib::Model> _foodModel;
            std::unique_ptr<raylib::Model> _linemateModel;
            std::unique_ptr<raylib::Model> _deraumereModel;
            std::unique_ptr<raylib::Model> _siburModel;
            std::unique_ptr<raylib::Model> _mendianeModel;
            std::unique_ptr<raylib::Model> _phirasModel;
            std::unique_ptr<raylib::Model> _thystameModel;

            std::map<std::pair<int, int>, GUI::TileInfo> _tileData;
            std::unordered_map<std::string, GUI::Player> _playerData;
            std::unordered_map<std::string, GUI::EggInfo> _eggData;

            void drawGround();
            void drawResources();
            void drawEggs();
            void drawPlayers();

        public:
            Map(std::size_t width, std::size_t height, float tileSize = 1.0f);
            ~Map() = default;

            std::size_t get_width() const { return _width; }
            std::size_t get_height() const { return _height; }
            void updateTileData(const std::map<std::pair<int, int>, GUI::TileInfo>& tiles);
            void updatePlayerData(const std::unordered_map<std::string, GUI::Player>& players);
            void updateEggData(const std::unordered_map<std::string, GUI::EggInfo>& eggs);
            void render();
    };
} // namespace GUI
