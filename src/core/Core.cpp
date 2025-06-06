#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include "Core.hpp"

GUI::Core::Core(char **argv)
{
    for (int i = 1; i <= 3; ++i) {
        std::string arg = argv[i];
        if (arg == "-p") {
            try {
                _port = std::stoi(argv[i + 1]);
            } catch (const std::invalid_argument &) {
                throw CoreError("Invalid port: not a number");
            } catch (const std::out_of_range &) {
                throw CoreError("Invalid port: number out of range");
            }
        } else if (arg == "-h")
            _hostname = argv[i + 1];
    }

    if (_port == 0 || _hostname.empty())
        throw CoreError("Missing -p or -h argument");
}

bool GUI::Core::connect_to_server()
{
    _server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_fd == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return false;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_port);
    
    if (inet_pton(AF_INET, _hostname.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        return false;
    }

    if (connect(_server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return false;
    }

    return true;
}

void GUI::Core::run()
{
    std::cout << "Connecting to " << _hostname << ":" << _port << std::endl;

    if (!connect_to_server())
        throw CoreError("Failed to connect to server");
}


int execute_zappygui(char **argv)
{
    try {
        GUI::Core core(argv);

        core.run();
    } catch (const GUI::Core::CoreError &error) {
        std::cerr << "Core error: " << error.what() << std::endl;
    }
    return 0;
}