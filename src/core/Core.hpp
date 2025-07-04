/*
** EPITECH PROJECT, 2025
** ZAPPY GUI
** File description:
** Core.hpp
*/

#pragma once

#include "../player/Player.hpp"
#include "../../include/raylib-cpp.hpp"
#include "../audio/Audio.hpp"
#include "../clock/Clock.hpp"
#include "../macros.hpp"
#include <exception>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace GUI {
    class NetworkManager;
    class CommunicationBuffer;

    struct TileInfo {
        int x, y;
        std::vector<int> resources;
    };

    struct DeathMessage {
        std::string text;
        double timestamp;
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
                    CoreError(std::string  message) : _message(std::move(message)) {}
                    [[nodiscard]] const char* what() const noexcept override { return _message.c_str(); }
            };

            Core(char **argv);
            ~Core();

            bool connect_to_server();
            void handle_server_message(const std::string &message);
            void send_command(const std::string& command);
            void run();

            void drawDeathMessages();

            GUI::AudioManager &getAudioManager() { 
                return _audio;
            }

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

            std::vector<DeathMessage> _deathMessages;

            void drawInfoOverlay();

            GUI::AudioManager _audio;
    };
} // namespace GUI

int execute_zappygui(char **argv);
