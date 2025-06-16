/*
** EPITECH PROJECT, 2025
** ZAPPY GUI
** File description:
** Core.hpp
*/

#pragma once

#include <string>
#include <memory>
#include <stdexcept>

namespace GUI {
    class NetworkManager;
    class CommunicationBuffer;
} // namespace GUI

namespace GUI {

    class Core {
    public:
        class CoreError : public std::runtime_error {
        public:
            CoreError(const std::string& message) : std::runtime_error(message) {}
        };

        Core(char **argv);
        ~Core();
        
        void run();
        void send_command(const std::string& command);

    private:
        bool connect_to_server();
        void handle_server_message(const std::string &message);
        
        std::unique_ptr<NetworkManager> _network_manager;
        std::unique_ptr<CommunicationBuffer> _comm_buffer;
        
        std::string _hostname;
        int _port;
        int _timeUnit;
        bool _connected;
        
        int _server_fd;
    };

} // namespace GUI

int execute_zappygui(char **argv);
