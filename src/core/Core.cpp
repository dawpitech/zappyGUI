#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <sstream>
#include "Core.hpp"

GUI::Core::Core(char **argv) : _server_fd(-1), _connected(false)
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

GUI::Core::~Core()
{
    if (_server_fd != -1)
        close(_server_fd);
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
    return authenticate();
}

bool GUI::Core::authenticate()
{
    char buffer[1024];
    
    ssize_t bytes_read = recv(_server_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) {
        std::cerr << "Failed to receive WELCOME" << std::endl;
        return false;
    }
    
    buffer[bytes_read] = '\0';
    std::cout << "Received: " << buffer;
    
    if (std::string(buffer) != "WELCOME\n") {
        std::cerr << "Expected WELCOME, got: " << buffer << std::endl;
        return false;
    }

    if (send(_server_fd, "GRAPHIC\n", 8, 0) != 8) {
        std::cerr << "Failed to send GRAPHIC" << std::endl;
        return false;
    }
    std::cout << "Successfully authenticated as GUI" << std::endl;
    _connected = true;
    return true;
}

void GUI::Core::handle_server_message(const std::string &message)
{
    if (message.empty())
        return;
    
    std::istringstream iss(message);
    std::string command;
    iss >> command;

    std::cout << "Received: " << message << std::endl;

    if (command == "msz") {
        int width;
        int height;
        iss >> width >> height;
        std::cout << "Map size: " << width << "x" << height << std::endl;
    } else if (command == "bct") {
        int x;
        int y;
        int q0;
        int q1;
        int q2;
        int q3;
        int q4;
        int q5;
        int q6;
        iss >> x >> y >> q0 >> q1 >> q2 >> q3 >> q4 >> q5 >> q6;
        std::cout << "Tile (" << x << "," << y << ") resources: "
                  << q0 << " " << q1 << " " << q2 << " " << q3 << " "
                  << q4 << " " << q5 << " " << q6 << std::endl;
    } else if (command == "tna") {
        std::string team_name;
        iss >> team_name;
        std::cout << "Team: " << team_name << std::endl;
    } else if (command == "pnw") {
        std::string player_id_str;
        std::string team_name;
        int x;
        int y;
        int orientation;
        int level;
        iss >> player_id_str >> x >> y >> orientation >> level >> team_name;
        std::cout << "Player " << player_id_str << " connected at (" 
                  << x << "," << y << ") team: " << team_name << std::endl;
    } else if (command == "ppo") {
        std::string player_id_str;
        int x;
        int y;
        int orientation;
        iss >> player_id_str >> x >> y >> orientation;
        std::cout << "Player " << player_id_str << " position: (" 
                  << x << "," << y << ") orientation: " << orientation << std::endl;
    } else if (command == "plv") {
        std::string player_id_str;
        int level;
        iss >> player_id_str >> level;
        std::cout << "Player " << player_id_str << " level: " << level << std::endl;
    } else if (command == "pin") {
        std::string player_id_str;
        int x;
        int y;
        int q0;
        int q1;
        int q2;
        int q3;
        int q4;
        int q5;
        int q6;
        iss >> player_id_str >> x >> y >> q0 >> q1 >> q2 >> q3 >> q4 >> q5 >> q6;
        std::cout << "Player " << player_id_str << " inventory at (" 
                  << x << "," << y << "): " << q0 << " " << q1 << " " << q2 
                  << " " << q3 << " " << q4 << " " << q5 << " " << q6 << std::endl;
    } else if (command == "pex") {
        std::string player_id_str;
        iss >> player_id_str;
        std::cout << "Player " << player_id_str << " expelled" << std::endl;
    } else if (command == "pbc") {
        std::string player_id_str;
        std::string broadcast_message;
        iss >> player_id_str;
        std::getline(iss, broadcast_message);
        std::cout << "Player " << player_id_str << " broadcast:" << broadcast_message << std::endl;
    } else if (command == "pic") {
        int x;
        int y;
        int level;
        iss >> x >> y >> level;
        std::cout << "Incantation started at (" << x << "," << y << ") level " << level;
        std::string player_id_str;
        while (iss >> player_id_str) {
            if (player_id_str[0] == '#')
                std::cout << " player " << player_id_str;
        }
        std::cout << std::endl;
    } else if (command == "pie") {
        int x;
        int y;
        int result;
        iss >> x >> y >> result;
        std::cout << "Incantation ended at (" << x << "," << y << ") result: " 
                  << (result != 0 ? "success" : "failure") << std::endl;
    } else if (command == "pfk") {
        std::string player_id_str;
        iss >> player_id_str;
        std::cout << "Player " << player_id_str << " laid an egg" << std::endl;
    } else if (command == "pdr") {
        std::string player_id_str;
        int resource;
        iss >> player_id_str >> resource;
        std::cout << "Player " << player_id_str << " dropped resource " << resource << std::endl;
    } else if (command == "pgt") {
        std::string player_id_str;
        int resource;
        iss >> player_id_str >> resource;
        std::cout << "Player " << player_id_str << " collected resource " << resource << std::endl;
    }  else if (command == "pdi") {
        std::string player_id_str;
        iss >> player_id_str;
        std::cout << "Player " << player_id_str << " died" << std::endl;
    } else if (command == "enw") {
        std::string egg_id_str;
        std::string player_id_str;
        int x;
        int y;
        iss >> egg_id_str >> player_id_str >> x >> y;
        std::cout << "New egg " << egg_id_str << " laid by " << player_id_str 
                  << " at (" << x << "," << y << ")" << std::endl;
    } else if (command == "ebo") {
        std::string egg_id_str;
        iss >> egg_id_str;
        std::cout << "Egg " << egg_id_str << " hatched" << std::endl;
    } else if (command == "edi") {
        std::string egg_id_str;
        iss >> egg_id_str;
        std::cout << "Egg " << egg_id_str << " died" << std::endl;
    } else if (command == "sgt") {
        int time_unit;
        iss >> time_unit;
        _timeUnit = time_unit;
        std::cout << "Time unit: " << time_unit << std::endl;
    } else if (command == "seg") {
        std::string team_name;
        iss >> team_name;
        std::cout << "Game ended, winner: " << team_name << std::endl;
    } else if (command == "smg") {
        std::string server_message;
        std::getline(iss, server_message);
        std::cout << "Server message:" << server_message << std::endl;
    } else if (command == "suc")
        std::cout << "Unknown command sent to server" << std::endl;
    else if (command == "sbp")
        std::cout << "Bad parameters sent to server" << std::endl;
    else
        std::cout << "Unknown message from server: " << message << std::endl;
}

void GUI::Core::send_command(const std::string& command)
{
    if (!_connected) {
        std::cerr << "Not connected to server" << std::endl;
        return;
    }
    
    std::string cmd = command + "\n";
    ssize_t sent = send(_server_fd, cmd.c_str(), cmd.length(), 0);
    if (sent != static_cast<ssize_t>(cmd.length()))
        std::cerr << "Failed to send command: " << command << std::endl;
}

void GUI::Core::run()
{
    std::cout << "Connecting to " << _hostname << ":" << _port << std::endl;

    if (!connect_to_server()) {
        throw CoreError("Failed to connect to server");
    }
    send_command("msz");
    send_command("mct"); 
    send_command("tna");
    send_command("sgt");
    struct pollfd pfd;
    pfd.fd = _server_fd;
    pfd.events = POLLIN;

    std::cout << "GUI connected and listening for server messages..." << std::endl;

    while (_connected) {
        int ret = poll(&pfd, 1, 100);
        
        if (ret > 0 && (pfd.revents & POLLIN)) {
            char buffer[4096];
            ssize_t bytes_read = recv(_server_fd, buffer, sizeof(buffer) - 1, 0);
            
            if (bytes_read <= 0) {
                std::cout << "Server disconnected" << std::endl;
                _connected = false;
                break;
            }
            
            buffer[bytes_read] = '\0';
            _input_buffer += buffer;
            
            size_t pos;
            while ((pos = _input_buffer.find('\n')) != std::string::npos) {
                std::string message = _input_buffer.substr(0, pos);
                _input_buffer = _input_buffer.substr(pos + 1);
                
                if (!message.empty()) {
                    handle_server_message(message);
                }
            }
        }
        
        if (ret < 0) {
            std::cerr << "Poll error" << std::endl;
            break;
        }
    }
}

int execute_zappygui(char **argv)
{
    try {
        GUI::Core core(argv);
        core.run();
    } catch (const GUI::Core::CoreError &error) {
        std::cerr << "Core error: " << error.what() << std::endl;
        return 1;
    }
    return 0;
}