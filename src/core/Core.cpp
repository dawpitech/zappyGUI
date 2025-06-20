/*
** EPITECH PROJECT, 2025
** ZAPPY GUI
** File description:
** core.cpp
*/

#include "Core.hpp"
#include "../communication/CommunicationBuffer.hpp"
#include "../network/NetworkManager.hpp"
#include <iostream>
#include <sstream>

GUI::Core::Core(char **argv) : _port(0), _timeUnit(0), _connected(false), _server_fd(-1)
{
    _network_manager = std::make_unique<NetworkManager>();
    _comm_buffer = std::make_unique<CommunicationBuffer>();

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
        } else if (arg == "-h") {
            _hostname = argv[i + 1];
        }
    }

    if (_port == 0 || _hostname.empty())
        throw CoreError("Missing -p or -h argument");
}

GUI::Core::~Core()
{
}

bool GUI::Core::connect_to_server()
{
    if (!_network_manager->create_and_connect(_hostname, _port))
        return false;

    if (!_network_manager->authenticate())
        return false;

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
    } else if (command == "pdi") {
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
    } else if (command == "suc") {
        std::cout << "Unknown command sent to server" << std::endl;
    } else if (command == "sbp") {
        std::cout << "Bad parameters sent to server" << std::endl;
    } else {
        std::cout << "Unknown message from server: " << message << std::endl;
    }
}

void GUI::Core::send_command(const std::string& command)
{
    if (!_network_manager->send_command(command))
        std::cerr << "Failed to send command: " << command << std::endl;
}

void GUI::Core::run()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    raylib::Window window(screenWidth, screenHeight, "Zappy GUI - 3D Grid");

    raylib::Camera3D camera(
        { 10.0f, 20.0f, 30.0f }, // position
        { 0.0f, 0.0f, 0.0f },    // target
        { 0.0f, 1.0f, 0.0f },    // up vector
        45.0f, CAMERA_PERSPECTIVE
    );

    SetTargetFPS(60);

    int mapWidth = 10;
    int mapHeight = 10;
    bool gridReady = false;

    std::cout << "Connecting to " << _hostname << ":" << _port << std::endl;
    if (!connect_to_server())
        throw CoreError("Failed to connect to server");

    send_command("msz");
    send_command("mct");
    send_command("tna");
    send_command("sgt");

    while (!window.ShouldClose()) {
        if (_connected && _network_manager->poll_for_data()) {
            char buffer[4096];
            ssize_t bytes_read = _network_manager->receive_data(buffer, sizeof(buffer) - 1);
            if (bytes_read <= 0) {
                std::cout << "Server disconnected" << std::endl;
                _connected = false;
                break;
            }

            buffer[bytes_read] = '\0';
            _comm_buffer->append_data(buffer);

            auto messages = _comm_buffer->extract_all_messages();
            for (const auto& message : messages) {
                std::istringstream iss(message);
                std::string command;
                iss >> command;

                if (command == "msz") {
                    iss >> mapWidth >> mapHeight;
                    std::cout << "Map size: " << mapWidth << "x" << mapHeight << std::endl;
                    gridReady = true;
                }

                handle_server_message(message);
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            camera.Update(CAMERA_ORBITAL);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        if (gridReady)
            DrawGrid3D(mapWidth, mapHeight, 1.0f);
        EndMode3D();

        DrawText("Hold right mouse button and drag to move camera", 10, 10, 20, DARKGRAY);
        EndDrawing();
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
