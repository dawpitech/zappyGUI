/*
** EPITECH PROJECT, 2025
** ZAPPY GUI
** File description:
** NetworkManager.cpp
*/

#include "NetworkManager.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

GUI::NetworkManager::NetworkManager() : _socket_fd(-1)
{
}

GUI::NetworkManager::~NetworkManager()
{
    close_connection();
}

bool GUI::NetworkManager::create_and_connect(const std::string& hostname, int port)
{
    _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket_fd == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return false;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, hostname.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address: " << hostname << std::endl;
        close_connection();
        return false;
    }

    if (connect(_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed to " << hostname << ":" << port << std::endl;
        close_connection();
        return false;
    }
    
    std::cout << "Successfully connected to " << hostname << ":" << port << std::endl;
    return true;
}

bool GUI::NetworkManager::authenticate()
{
    if (_socket_fd == -1) {
        std::cerr << "Cannot authenticate: not connected" << std::endl;
        return false;
    }
    
    char buffer[1024];
    ssize_t bytes_read = recv(_socket_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) {
        std::cerr << "Failed to receive WELCOME message" << std::endl;
        return false;
    }
    
    buffer[bytes_read] = '\0';
    std::cout << "Received: " << buffer;
    
    if (std::string(buffer) != "WELCOME\n") {
        std::cerr << "Expected WELCOME, got: " << buffer << std::endl;
        return false;
    }

    const char* graphic_cmd = "GRAPHIC\n";
    if (send(_socket_fd, graphic_cmd, 8, 0) != 8) {
        std::cerr << "Failed to send GRAPHIC command" << std::endl;
        return false;
    }
    
    std::cout << "Successfully authenticated as GUI client" << std::endl;
    return true;
}

bool GUI::NetworkManager::send_command(const std::string& command)
{
    if (_socket_fd == -1) {
        std::cerr << "Cannot send command: not connected" << std::endl;
        return false;
    }
    
    std::string cmd = command + "\n";
    ssize_t sent = send(_socket_fd, cmd.c_str(), cmd.length(), 0);
    if (sent != static_cast<ssize_t>(cmd.length())) {
        std::cerr << "Failed to send command: " << command << std::endl;
        return false;
    }
    
    return true;
}

ssize_t GUI::NetworkManager::receive_data(char* buffer, size_t buffer_size)
{
    if (_socket_fd == -1) {
        return -1;
    }
    
    return recv(_socket_fd, buffer, buffer_size - 1, 0);
}

bool GUI::NetworkManager::poll_for_data(int timeout_ms)
{
    if (_socket_fd == -1) {
        return false;
    }
    
    struct pollfd pfd;
    pfd.fd = _socket_fd;
    pfd.events = POLLIN;
    
    int ret = poll(&pfd, 1, timeout_ms);
    if (ret < 0) {
        std::cerr << "Poll error on socket" << std::endl;
        return false;
    }
    
    return ret > 0 && ((pfd.revents & POLLIN) != 0);
}

void GUI::NetworkManager::close_connection()
{
    if (_socket_fd != -1) {
        close(_socket_fd);
        _socket_fd = -1;
        std::cout << "Connection closed" << std::endl;
    }
}

bool GUI::NetworkManager::is_connected() const
{
    return _socket_fd != -1;
}
