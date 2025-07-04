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
#include <utility>
#include <vector>
#include <map>
#include "../player/Player.hpp"
#include <unordered_map>
#include "../../include/raylib-cpp.hpp"
#include "../clock/Clock.hpp"
#include "../audio/Audio.hpp"

namespace GUI {
    class NetworkManager;
    class CommunicationBuffer;

    struct IncantationInfo {
        int x;
        int y;
        int level;
        float start_time;
        std::vector<std::string> player_ids;
    };

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
        std::map<std::pair<int, int>, IncantationInfo> activeIncantations;
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

            const std::map<std::pair<int, int>, IncantationInfo> &getActiveIncantations() const {
                return _gameInfo.activeIncantations;
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
