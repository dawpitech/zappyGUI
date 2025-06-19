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
#include "../map/Map.hpp"

GUI::Core::Core(char **argv) : _port(0), _timeUnit(0), _connected(false), _server_fd(-1), _showInfoOverlay(false)
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
        _mapInfo.width = width;
        _mapInfo.height = height;
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

        TileInfo tile;
        tile.x = x;
        tile.y = y;
        tile.resources = {q0, q1, q2, q3, q4, q5, q6};
        _mapInfo.tiles[{x, y}] = tile;

        std::cout << "Tile (" << x << "," << y << ") resources: "
                  << q0 << " " << q1 << " " << q2 << " " << q3 << " "
                  << q4 << " " << q5 << " " << q6 << std::endl;
    } else if (command == "tna") {
        std::string team_name;
        iss >> team_name;
        _gameInfo.teams.push_back(team_name);
        std::cout << "Team: " << team_name << std::endl;
    } else if (command == "pnw") {
        std::string player_id_str;
        std::string team_name;
        int x;
        int y;
        int orientation;
        int level;
        iss >> player_id_str >> x >> y >> orientation >> level >> team_name;

        PlayerInfo player;
        player.id = player_id_str;
        player.x = x;
        player.y = y;
        player.orientation = orientation;
        player.level = level;
        player.team = team_name;
        _gameInfo.players[player_id_str] = player;

        std::cout << "Player " << player_id_str << " connected at ("
                  << x << "," << y << ") team: " << team_name << std::endl;
    } else if (command == "ppo") {
        std::string player_id_str;
        int x;
        int y;
        int orientation;
        iss >> player_id_str >> x >> y >> orientation;

        if (_gameInfo.players.find(player_id_str) != _gameInfo.players.end()) {
            _gameInfo.players[player_id_str].x = x;
            _gameInfo.players[player_id_str].y = y;
            _gameInfo.players[player_id_str].orientation = orientation;
        }

        std::cout << "Player " << player_id_str << " position: ("
                  << x << "," << y << ") orientation: " << orientation << std::endl;
    } else if (command == "plv") {
        std::string player_id_str;
        int level;
        iss >> player_id_str >> level;

        if (_gameInfo.players.find(player_id_str) != _gameInfo.players.end()) {
            _gameInfo.players[player_id_str].level = level;
        }

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

        if (_gameInfo.players.find(player_id_str) != _gameInfo.players.end()) {
            _gameInfo.players[player_id_str].inventory = {q0, q1, q2, q3, q4, q5, q6};
        }

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
        _gameInfo.players.erase(player_id_str);
        std::cout << "Player " << player_id_str << " died" << std::endl;
    } else if (command == "enw") {
        std::string egg_id_str;
        std::string player_id_str;
        int x;
        int y;
        iss >> egg_id_str >> player_id_str >> x >> y;

        EggInfo egg;
        egg.id = egg_id_str;
        egg.player_id = player_id_str;
        egg.x = x;
        egg.y = y;
        _gameInfo.eggs[egg_id_str] = egg;

        std::cout << "New egg " << egg_id_str << " laid by " << player_id_str
                  << " at (" << x << "," << y << ")" << std::endl;
    } else if (command == "ebo") {
        std::string egg_id_str;
        iss >> egg_id_str;
        _gameInfo.eggs.erase(egg_id_str);
        std::cout << "Egg " << egg_id_str << " hatched" << std::endl;
    } else if (command == "edi") {
        std::string egg_id_str;
        iss >> egg_id_str;
        _gameInfo.eggs.erase(egg_id_str);
        std::cout << "Egg " << egg_id_str << " died" << std::endl;
    } else if (command == "sgt") {
        int time_unit;
        iss >> time_unit;
        _timeUnit = time_unit;
        _gameInfo.timeUnit = time_unit;
        std::cout << "Time unit: " << time_unit << std::endl;
    } else if (command == "seg") {
        std::string team_name;
        iss >> team_name;
        _gameInfo.winner = team_name;
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

void GUI::Core::drawInfoOverlay()
{
    if (!_showInfoOverlay) return;

    const int screenWidth = 1280;
    const int screenHeight = 720;
    const int overlayWidth = 400;
    const int overlayHeight = 600;
    const int overlayX = screenWidth - overlayWidth - 20;
    const int overlayY = 20;

    DrawRectangle(overlayX, overlayY, overlayWidth, overlayHeight, Fade(BLACK, 0.8f));
    DrawRectangleLines(overlayX, overlayY, overlayWidth, overlayHeight, WHITE);

    int yOffset = overlayY + 20;
    const int lineHeight = 20;

    DrawText("GAME INFORMATION", overlayX + 10, yOffset, 20, YELLOW);
    yOffset += lineHeight * 2;

    DrawText("MAP INFO:", overlayX + 10, yOffset, 16, WHITE);
    yOffset += lineHeight;
    DrawText(TextFormat("Size: %dx%d", _mapInfo.width, _mapInfo.height), overlayX + 20, yOffset, 14, LIGHTGRAY);
    yOffset += lineHeight;
    DrawText(TextFormat("Tiles: %d", (int)_mapInfo.tiles.size()), overlayX + 20, yOffset, 14, LIGHTGRAY);
    yOffset += lineHeight * 2;

    DrawText("GAME INFO:", overlayX + 10, yOffset, 16, WHITE);
    yOffset += lineHeight;
    DrawText(TextFormat("Time Unit: %d", _gameInfo.timeUnit), overlayX + 20, yOffset, 14, LIGHTGRAY);
    yOffset += lineHeight;
    DrawText(TextFormat("Teams: %d", (int)_gameInfo.teams.size()), overlayX + 20, yOffset, 14, LIGHTGRAY);
    yOffset += lineHeight;
    DrawText(TextFormat("Players: %d", (int)_gameInfo.players.size()), overlayX + 20, yOffset, 14, LIGHTGRAY);
    yOffset += lineHeight;
    DrawText(TextFormat("Eggs: %d", (int)_gameInfo.eggs.size()), overlayX + 20, yOffset, 14, LIGHTGRAY);
    yOffset += lineHeight;

    if (!_gameInfo.winner.empty()) {
        DrawText(TextFormat("Winner: %s", _gameInfo.winner.c_str()), overlayX + 20, yOffset, 14, GREEN);
        yOffset += lineHeight;
    }
    yOffset += lineHeight;

    DrawText("TEAMS:", overlayX + 10, yOffset, 16, WHITE);
    yOffset += lineHeight;
    for (const auto& team : _gameInfo.teams) {
        DrawText(team.c_str(), overlayX + 20, yOffset, 14, LIGHTGRAY);
        yOffset += lineHeight;
    }
    yOffset += lineHeight;

    DrawText("PLAYERS:", overlayX + 10, yOffset, 16, WHITE);
    yOffset += lineHeight;
    int playerCount = 0;
    for (const auto& [id, player] : _gameInfo.players) {
        if (playerCount >= 10) {
            DrawText("...", overlayX + 20, yOffset, 14, LIGHTGRAY);
            break;
        }
        DrawText(TextFormat("%s (%s) Lv.%d",
                 player.id.c_str(),
                 player.team.c_str(),
                 player.level),
                 overlayX + 20, yOffset, 12, LIGHTGRAY);
        yOffset += lineHeight;
        playerCount++;
    }

    DrawText("Press 'I' to close", overlayX + 10, overlayY + overlayHeight - 30, 14, YELLOW);
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
    float zoom = 30.0f;
    const float minZoom = 5.0f;
    const float maxZoom = 100.0f;

    raylib::Window window(screenWidth, screenHeight, "Zappy GUI - 3D Grid");


    SetTargetFPS(60);

    int mapWidth = 10;
    int mapHeight = 10;
    bool gridReady = false;
    GUI::Map map(mapWidth, mapHeight, 1.0f);

    raylib::Camera3D camera(
        {10.0f, 20.0f, 30.0f},  // position
        {(float)mapWidth / 2, (float)mapHeight / 2, 0.0f},     // target
        {0.0f, 1.0f, 0.0f},     // up vector
        45.0f, CAMERA_PERSPECTIVE
    );

    std::cout << "Connecting to " << _hostname << ":" << _port << std::endl;
    if (!connect_to_server())
        throw CoreError("Failed to connect to server");

    send_command("msz");
    send_command("mct");
    send_command("tna");
    send_command("sgt");

    while (!window.ShouldClose())
    {
        float wheelMove = raylib::Mouse::GetWheelMove();
        if (wheelMove != 0) {
            zoom -= wheelMove * 2.0f;
            if (zoom < minZoom) zoom = minZoom;
            if (zoom > maxZoom) zoom = maxZoom;
            Vector3 dir = Vector3Normalize(Vector3Subtract(camera.position, camera.target));
            camera.position = Vector3Add(camera.target, Vector3Scale(dir, zoom));
        }

        camera.up = { 0.0f, 1.0f, 0.0f };
        camera.fovy = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;

        if (raylib::Keyboard::IsKeyPressed(KEY_I))
            _showInfoOverlay = !_showInfoOverlay;

        if (_connected && _network_manager->poll_for_data())
        {
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
                    map = GUI::Map(mapWidth, mapHeight, 1.0f);

                    camera.target = {(float)mapWidth / 2, 0.0f, (float)mapHeight / 2};
                }

                handle_server_message(message);
            }
        }

        if (raylib::Mouse::IsButtonDown(MOUSE_BUTTON_RIGHT))
            camera.Update(CAMERA_ORBITAL);

        window.BeginDrawing();
        window.ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        if (gridReady)
        {
            map.updateTileData(_mapInfo.tiles);
            map.updatePlayerData(_gameInfo.players);
            map.updateEggData(_gameInfo.eggs);
            map.render();
        }

        EndMode3D();

        DrawText("Hold right mouse button and drag to move camera", 10, 10, 20, DARKGRAY);
        DrawText("Press 'I' to toggle information overlay", 10, 35, 20, DARKGRAY);

        if (_showInfoOverlay)
            drawInfoOverlay();

        window.EndDrawing();
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
