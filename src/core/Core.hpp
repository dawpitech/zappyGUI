/*
** EPITECH PROJECT, 2025
** ZAPPY GUI
** File description:
** Core.hpp
*/

#pragma once

#include <string>
#include <memory>
#include <exception>
#include <vector>
#include <map>
#include "../player/Player.hpp"
#include <unordered_map>
#include "../../include/raylib-cpp.hpp"
#include "../map/Map.hpp"
#include "../clock/Clock.hpp"

namespace GUI {
    class NetworkManager;
    class CommunicationBuffer;

    struct TileInfo {
        int x, y;
        std::vector<int> resources;
    };

    struct EggInfo {
        std::string id;
        std::string player_id;
        int x, y;
    };

    struct MapInfo {
        int width = 0;
        int height = 0;
        std::map<std::pair<int, int>, TileInfo> tiles;
    };

    struct GameInfo {
        int timeUnit = 0;
        std::vector<std::string> teams;
        std::unordered_map<std::string, Player> players;
        std::unordered_map<std::string, EggInfo> eggs;
        std::string winner;
    };

    class Core {
        public:
            class CoreError : public std::exception {
                private:
                    std::string _message;
                public:
                    CoreError(const std::string& message) : _message(message) {}
                    const char* what() const noexcept override { return _message.c_str(); }
            };

            Core(char **argv);
            ~Core();

            bool connect_to_server();
            void handle_server_message(const std::string &message);
            void send_command(const std::string& command);
            void run();

        private:
            std::unique_ptr<NetworkManager> _network_manager;
            std::unique_ptr<CommunicationBuffer> _comm_buffer;
            std::string _hostname;
            int _port;
            int _timeUnit;
            bool _connected;
            int _server_fd;
            
            bool _showInfoOverlay;
            MapInfo _mapInfo;
            GameInfo _gameInfo;

            std::unique_ptr<Clock> _clock;

            void drawInfoOverlay();
    };
}

int execute_zappygui(char **argv);
