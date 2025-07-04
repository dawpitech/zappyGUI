/*
** EPITECH PROJECT, 2025
** ZAPPY GUI
** File description:
** NetworkManager.hpp
*/

#include <string>
#include "../macros.hpp"

namespace GUI {

    class NetworkManager {
        public:
            NetworkManager();
            ~NetworkManager();
            
            NetworkManager(const NetworkManager&) = delete;
            NetworkManager& operator=(const NetworkManager&) = delete;
            
            bool create_and_connect(const std::string& hostname, int port);
            bool authenticate();
            bool send_command(const std::string& command);
            
            ssize_t receive_data(char* buffer, size_t buffer_size);
            
            bool poll_for_data(int timeout_ms = 100);
            
            void close_connection();
            
            [[nodiscard]] bool is_connected() const;
            
        private:
            int _socket_fd;
    };
} // namespace GUI
