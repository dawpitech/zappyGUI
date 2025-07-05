/**
 * @file Core.cpp
 * @brief Implementation of the Core class for the Zappy project GUI
 * @author EPITECH PROJECT, 2025
 * @date 2025
 *
 * This file contains the implementation of the Core class which manages the core of the
 * Zappy GUI application. It includes network management, 3D rendering, and user interface.
 */

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
#include <raylib.h>
#include <sstream>
#include "../map/Map.hpp"

/**
 * @brief Constructor of the Core class
 * @param argv Array of command line arguments
 *
 * Initializes the Core with connection parameters (-p for port, -h for hostname).
 * Creates network, communication and clock managers.
 *
 * @throw CoreError If -p or -h arguments are missing or invalid
 */
GUI::Core::Core(char **argv) : _port(0), _timeUnit(0), _connected(false), _server_fd(-1), _showInfoOverlay(false)
{
    _network_manager = std::make_unique<NetworkManager>();
    _comm_buffer = std::make_unique<CommunicationBuffer>();
    _clock = std::make_unique<Clock>();

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

/**
 * @brief Destructor of the Core class
 *
 * Cleans up resources used by the Core.
 */
GUI::Core::~Core()
{
}

/**
 * @brief Establishes connection to the Zappy server
 * @return true if connection and authentication succeed, false otherwise
 *
 * Uses the NetworkManager to create a connection to the specified server
 * and performs the required authentication.
 */
bool GUI::Core::connect_to_server()
{
    if (!_network_manager->create_and_connect(_hostname, _port))
        return false;

    if (!_network_manager->authenticate())
        return false;

    _connected = true;
    return true;
}

/**
 * @brief Processes messages received from the server
 * @param message Message received from the server to process
 *
 * Parses and processes all types of messages from the Zappy protocol:
 * - msz : Map size
 * - bct : Tile content
 * - tna : Team name
 * - pnw : New player
 * - ppo : Player position
 * - plv : Player level
 * - pin : Player inventory
 * - pex : Player expulsion
 * - pbc : Broadcast message
 * - pic : Incantation start
 * - pie : Incantation end
 * - pfk : Egg laying
 * - pdr : Resource drop
 * - pgt : Resource collection
 * - pdi : Player death
 * - enw : New egg
 * - ebo : Egg hatching
 * - edi : Egg death
 * - sgt : Time unit
 * - seg : End of game
 * - smg : Server message
 * - suc : Unknown command
 * - sbp : Bad parameters
 */
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

        _audio.play("newPlayer");
        GUI::Player player(x, y, player_id_str, team_name, orientation, level);
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
            _gameInfo.players[player_id_str].addMove(x - _gameInfo.players[player_id_str].getX(), y - _gameInfo.players[player_id_str].getY());
            _gameInfo.players[player_id_str].setPosition(x, y);
            _gameInfo.players[player_id_str].setOrientation(orientation);
        }

        std::cout << "Player " << player_id_str << " position: ("
                  << x << "," << y << ") orientation: " << orientation << std::endl;
    } else if (command == "plv") {
        std::string player_id_str;
        int level;
        iss >> player_id_str >> level;

        if (_gameInfo.players.find(player_id_str) != _gameInfo.players.end()) {
            _gameInfo.players[player_id_str].setLevel(level);
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

        if (_gameInfo.players.find(player_id_str) != _gameInfo.players.end())
            _gameInfo.players[player_id_str].setInventory({q0, q1, q2, q3, q4, q5, q6});

        std::cout << "Player " << player_id_str << " inventory at ("
                  << x << "," << y << "): " << q0 << " " << q1 << " " << q2
                  << " " << q3 << " " << q4 << " " << q5 << " " << q6 << std::endl;
    } else if (command == "pex") {
        _audio.play("playerExpulsion");
        std::string player_id_str;
        iss >> player_id_str;
        std::cout << "Player " << player_id_str << " expelled" << std::endl;
    } else if (command == "pbc") {
        _audio.play("broadcast");
        std::string idStr;
        std::string msg;

        iss >> idStr;
        std::getline(iss, msg);
        if (!msg.empty() && msg[0] == ' ')
            msg.erase(0, 1);
        auto it = _gameInfo.players.find(idStr);
        if (it != _gameInfo.players.end())
            it->second.setBroadcastMessage(msg);
    } else if (command == "pic") {
        _audio.play("incantationStart");
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
        _audio.play("incantationEnd");
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
        _audio.play("deathPlayer");
        std::string player_id_str;
        iss >> player_id_str;
        DeathMessage msg;
        msg.text = TextFormat("Player %s died", player_id_str.c_str());
        msg.timestamp = GetTime();
        _deathMessages.push_back(msg);
        _gameInfo.players.erase(player_id_str);
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
        _clock->setTimeUnit(time_unit);
        std::cout << "Time unit: " << time_unit << std::endl;
    } else if (command == "seg") {
        _audio.play("endGame");
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

/**
 * @brief Displays the game information overlay
 *
 * Draws a user interface overlay containing:
 * - Map information (size, number of tiles)
 * - Game information (time unit, teams, players, eggs)
 * - Teams list
 * - Players list (limited to 10 for display)
 * - Game winner if any
 *
 * The overlay is displayed only if _showInfoOverlay is true.
 */
void GUI::Core::drawInfoOverlay()
{
    if (!_showInfoOverlay) return;

    const int screenWidth = WINDOW_WIDTH;
    const int screenHeight = WINDOW_HEIGHT;
    const int overlayWidth = OVERLAY_WIDTH;
    const int overlayHeight = OVERLAY_HEIGHT;
    const int overlayX = screenWidth - overlayWidth - 20;
    const int overlayY = 20;

    DrawRectangle(overlayX, overlayY, overlayWidth, overlayHeight, Fade(BLACK, 0.8F));
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
                 player.getName().c_str(),
                 player.getTeam().c_str(),
                 player.getLevel()),
                 overlayX + 20, yOffset, 12, LIGHTGRAY);
        yOffset += lineHeight;
        playerCount++;
    }

    DrawText("Press 'I' to close", overlayX + 10, overlayY + overlayHeight - 30, 14, YELLOW);
}

/**
 * @brief Sends a command to the server
 * @param command Command to send to the server
 *
 * Uses the NetworkManager to send the specified command to the server.
 * Displays an error message if sending fails.
 */
void GUI::Core::send_command(const std::string& command)
{
    if (!_network_manager->send_command(command))
        std::cerr << "Failed to send command: " << command << std::endl;
}

/**
 * @brief Displays player death messages
 *
 * Draws player death messages on screen for 5 seconds.
 * Messages are automatically removed after this delay.
 * Messages are displayed in red starting from position (45, 500).
 */
void GUI::Core::drawDeathMessages()
{
    double currentTime = GetTime();
    int y = 500;

    for (auto it = _deathMessages.begin(); it != _deathMessages.end(); ) {
        if (currentTime - it->timestamp > 5.0F)
            it = _deathMessages.erase(it);
        else {
            DrawText(it->text.c_str(), 45, y, 20, RED);
            y += 25;
            ++it;
        }
    }
}

/**
 * @brief Main execution function of the GUI
 *
 * Starts the main game loop which:
 * 1. Initializes the Raylib window (1280x720)
 * 2. Creates the 3D camera with orbital controls
 * 3. Connects to the server and sends initial commands
 * 4. Executes the main rendering loop which:
 *    - Handles user input (zoom, camera rotation, overlay)
 *    - Receives and processes server messages
 *    - Updates and displays the 3D map
 *    - Displays the user interface
 *    - Handles death messages
 *
 * Controls:
 * - Mouse wheel: Zoom in/out
 * - Right click + drag: Camera rotation
 * - I key: Toggle information overlay
 *
 * @throw CoreError If server connection fails
 */
void GUI::Core::run()
{
    const int screenWidth = WINDOW_WIDTH;
    const int screenHeight = WINDOW_HEIGHT;
    float zoom = 30.0F;
    const float minZoom = 5.0F;
    const float maxZoom = 100.0F;

    raylib::Window window(screenWidth, screenHeight, "Zappy-Pi");
    Model backgroundModel;
    
    initializeWindow(backgroundModel);

    int mapWidth = 10;
    int mapHeight = 10;
    bool gridReady = false;

    std::unique_ptr<GUI::Map> map = std::make_unique<GUI::Map>(mapWidth, mapHeight, 1.0f);

    raylib::Camera3D camera(
        {10.0F, 20.0F, 30.0F},
        {(float)mapWidth / 2, (float)mapHeight / 2, 0.0F},
        {0.0F, 1.0F, 0.0F},
        45.0F, CAMERA_PERSPECTIVE
    );

    connectToServer();
    sendInitialCommands();

    while (!raylib::Window::ShouldClose())
    {
        for (auto &player : this->_gameInfo.players)
            send_command("ppo " + player.first);

        handleCameraZoom(camera, zoom, minZoom, maxZoom);
        handleKeyboardInput();
        processNetworkData(mapWidth, mapHeight, gridReady, map, camera);
        handleMouseCamera(camera);
        renderScene(window, camera, backgroundModel, map, gridReady);
        renderUI(window, map, camera, gridReady);
    }
}

void GUI::Core::initializeWindow(Model &backgroundModel)
{
    backgroundModel = LoadModel("assets/background.glb");
    SetTargetFPS(60);
}

void GUI::Core::initializeCamera(raylib::Camera3D &camera, int mapWidth, int mapHeight, float zoom)
{
    camera.up = { 0.0F, 1.0F, 0.0F };
    camera.fovy = 45.0F;
    camera.projection = CAMERA_PERSPECTIVE;
    camera.target = {(float)mapWidth / 2, 0.0F, (float)mapHeight / 2};
    
    Vector3 dir = Vector3Normalize(Vector3Subtract(camera.position, camera.target));
    camera.position = Vector3Add(camera.target, Vector3Scale(dir, zoom));
}

void GUI::Core::connectToServer()
{
    std::cout << "Connecting to " << _hostname << ":" << _port << std::endl;
    if (!connect_to_server())
        throw CoreError("Failed to connect to server");
}

void GUI::Core::sendInitialCommands()
{
    send_command("msz");
    send_command("mct");
    send_command("tna");
    send_command("sgt");
}

void GUI::Core::handleCameraZoom(raylib::Camera3D &camera, float &zoom, float minZoom, float maxZoom)
{
    float wheelMove = raylib::Mouse::GetWheelMove();
    if (wheelMove != 0) {
        zoom -= wheelMove * 2.0F;
        if (zoom < minZoom) zoom = minZoom;
        if (zoom > maxZoom) zoom = maxZoom;
        Vector3 dir = Vector3Normalize(Vector3Subtract(camera.position, camera.target));
        camera.position = Vector3Add(camera.target, Vector3Scale(dir, zoom));
    }

    camera.up = { 0.0F, 1.0F, 0.0F };
    camera.fovy = 45.0F;
    camera.projection = CAMERA_PERSPECTIVE;
}

void GUI::Core::handleKeyboardInput()
{
    if (raylib::Keyboard::IsKeyPressed(KEY_I))
        _showInfoOverlay = !_showInfoOverlay;
}

void GUI::Core::processNetworkData(int &mapWidth, int &mapHeight, bool &gridReady, 
                                  std::unique_ptr<GUI::Map> &map, raylib::Camera3D &camera)
{
    if (!_connected || !_network_manager->poll_for_data())
        return;

    char buffer[NETWORK_BUFFER_SIZE];
    ssize_t bytes_read = _network_manager->receive_data(buffer, sizeof(buffer) - 1);
    
    if (bytes_read <= 0) {
        std::cout << "Server disconnected" << std::endl;
        _connected = false;
        return;
    }

    buffer[bytes_read] = '\0';
    _comm_buffer->append_data(buffer);

    auto messages = _comm_buffer->extract_all_messages();
    for (const auto& message : messages)
        processServerMessage(message, mapWidth, mapHeight, gridReady, map, camera);
}

void GUI::Core::processServerMessage(const std::string &message, int &mapWidth, int &mapHeight, 
                                    bool &gridReady, std::unique_ptr<GUI::Map> &map, 
                                    raylib::Camera3D &camera)
{
    std::istringstream iss(message);
    std::string command;
    iss >> command;

    if (command == "msz") {
        iss >> mapWidth >> mapHeight;
        std::cout << "Map size: " << mapWidth << "x" << mapHeight << std::endl;
        gridReady = true;

        map = std::make_unique<GUI::Map>(mapWidth, mapHeight, 1.0F);
        camera.target = {(float)mapWidth / 2, 0.0F, (float)mapHeight / 2};
    }

    handle_server_message(message);
}

void GUI::Core::handleMouseCamera(raylib::Camera3D &camera)
{
    if (raylib::Mouse::IsButtonDown(MOUSE_BUTTON_RIGHT))
        camera.Update(CAMERA_ORBITAL);
}

void GUI::Core::renderScene(raylib::Window &window, raylib::Camera3D &camera, Model &backgroundModel, 
                           std::unique_ptr<GUI::Map> &map, bool gridReady)
{
    window.BeginDrawing();
    window.ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    DrawModel(backgroundModel, { 0.0F, -50.0F, 0.0F }, 0.5F, WHITE);
    
    if (gridReady) {
        map->updateTileData(_mapInfo.tiles);
        map->updatePlayerData(_gameInfo.players);
        map->updateEggData(_gameInfo.eggs);
        map->render();
    }

    EndMode3D();
}

void GUI::Core::renderUI(raylib::Window &window, std::unique_ptr<GUI::Map> &map, raylib::Camera3D &camera, bool gridReady)
{
    if (gridReady)
        map->renderUI(camera);

    DrawText(TextFormat("Timer: %.2f", _clock->getElapsedSeconds()), 35, 60, 20, RED);
    DrawText("Hold right mouse button and drag to move camera", 10, 10, 20, DARKGRAY);
    DrawText("Press 'I' to toggle information overlay", 10, 35, 20, DARKGRAY);

    if (_showInfoOverlay)
        drawInfoOverlay();

    drawDeathMessages();

    window.EndDrawing();
}

/**
 * @brief Main entry point for Zappy GUI execution
 * @param argv Array of command line arguments
 * @return 0 on success, 1 on error
 *
 * Wrapper function that:
 * 1. Creates a Core instance with the provided arguments
 * 2. Starts GUI execution
 * 3. Catches and displays any errors
 *
 * Expected arguments:
 * - -p <port> : Server port
 * - -h <hostname> : Server hostname
 */
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
