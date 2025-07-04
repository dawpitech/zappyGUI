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

/**
 * @brief Default constructor for NetworkManager
 *
 * Initializes the socket file descriptor to -1 (invalid state)
 */
GUI::NetworkManager::NetworkManager() : _socket_fd(-1)
{
}

/**
 * @brief Destructor for NetworkManager
 *
 * Ensures proper cleanup by closing any open connection
 */
GUI::NetworkManager::~NetworkManager()
{
    close_connection();
}

/**
 * @brief Creates a socket and establishes connection to the specified server
 *
 * This method creates a TCP socket, configures the server address, and attempts
 * to connect to the specified hostname and port. Only supports IPv4 addresses.
 *
 * @param hostname The IP address of the server to connect to (IPv4 format)
 * @param port The port number to connect to
 * @return true if connection is successfully established, false otherwise
 *
 * @note This method expects hostname to be a valid IPv4 address string
 * @note Any existing connection will be closed if this method fails
 */
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

/**
 * @brief Performs authentication handshake with the server
 *
 * This method handles the authentication protocol for GUI clients:
 * 1. Receives and validates the "WELCOME\n" message from the server
 * 2. Sends the "GRAPHIC\n" command to identify as a GUI client
 *
 * @return true if authentication is successful, false otherwise
 *
 * @note Requires an active connection (socket must be valid)
 * @note The method expects exactly "WELCOME\n" message from the server
 * @note Sends exactly 8 bytes for the "GRAPHIC\n" command
 */
bool GUI::NetworkManager::authenticate()
{
    if (_socket_fd == -1) {
        std::cerr << "Cannot authenticate: not connected" << std::endl;
        return false;
    }

    char buffer[NETWORK_BUFFER_SIZE];
    ssize_t bytes_read = recv(_socket_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) {
        std::cerr << "Failed to receive WELCOME message" << std::endl;
        return false;
    }

    buffer[bytes_read] = '\0';
    std::cout << "Received: " << buffer;

    if (std::string(buffer) != WELCOME_MSG) {
        std::cerr << "Expected WELCOME, got: " << buffer << std::endl;
        return false;
    }

    const char *graphic_cmd = GRAPHIC_MSG;
    if (send(_socket_fd, graphic_cmd, 8, 0) != 8) {
        std::cerr << "Failed to send GRAPHIC command" << std::endl;
        return false;
    }

    std::cout << "Successfully authenticated as GUI client" << std::endl;
    return true;
}

/**
 * @brief Sends a command to the connected server
 *
 * Appends a newline character to the command and sends it through the socket.
 * The method ensures the entire command is sent by checking the return value.
 *
 * @param command The command string to send (without trailing newline)
 * @return true if the command was successfully sent, false otherwise
 *
 * @note Requires an active connection (socket must be valid)
 * @note A newline character is automatically appended to the command
 * @note The method verifies that all bytes were sent successfully
 */
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

/**
 * @brief Receives data from the connected server
 *
 * Attempts to read data from the socket into the provided buffer.
 * The method reserves one byte for null termination.
 *
 * @param buffer Pointer to the buffer where received data will be stored
 * @param buffer_size Size of the buffer in bytes
 * @return Number of bytes received, or -1 if not connected or on error
 *
 * @note Requires an active connection (socket must be valid)
 * @note The method reserves one byte in the buffer for null termination
 * @note Returns -1 if no connection is established
 * @note The caller is responsible for null-terminating the received data if needed
 */
ssize_t GUI::NetworkManager::receive_data(char* buffer, size_t buffer_size)
{
    if (_socket_fd == -1)
        return -1;

    return recv(_socket_fd, buffer, buffer_size - 1, 0);
}

/**
 * @brief Polls the socket for incoming data with a timeout
 *
 * Uses the poll() system call to check if data is available for reading
 * on the socket without blocking indefinitely.
 *
 * @param timeout_ms Timeout in milliseconds (0 for immediate return, -1 for infinite wait)
 * @return true if data is available for reading, false if timeout occurred, no data available, or error
 *
 * @note Requires an active connection (socket must be valid)
 * @note Returns false on poll errors or if no connection exists
 * @note A timeout of 0 makes the call non-blocking
 * @note A timeout of -1 makes the call block indefinitely until data arrives
 */
bool GUI::NetworkManager::poll_for_data(int timeout_ms)
{
    if (_socket_fd == -1) 
        return false;

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

/**
 * @brief Closes the network connection
 *
 * Safely closes the socket file descriptor and resets it to -1.
 * This method can be called multiple times safely.
 *
 * @note Safe to call even if no connection is established
 * @note After calling this method, is_connected() will return false
 * @note Prints a confirmation message when a connection is actually closed
 */
void GUI::NetworkManager::close_connection()
{
    if (_socket_fd != -1) {
        close(_socket_fd);
        _socket_fd = -1;
        std::cout << "Connection closed" << std::endl;
    }
}

/**
 * @brief Checks if a connection is currently established
 *
 * @return true if connected (socket is valid), false otherwise
 *
 * @note This method only checks if the socket file descriptor is valid
 * @note It does not verify if the connection is still active on the network level
 */
bool GUI::NetworkManager::is_connected() const
{
    return _socket_fd != -1;
}
