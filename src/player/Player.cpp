/**
 * @file Player.cpp
 * @brief Implementation of the Player class for the ZappyGUI project
 * @author EPITECH PROJECT, 2025
 * @date 2025
 *
 * This file contains the implementation of the Player class, which manages
 * all aspects of a player entity within the ZappyGUI application. It handles
 * player state including position, orientation, level, team affiliation,
 * inventory, and broadcast messages.
 *
 * The class provides methods to get and set player attributes, track
 * movement offsets, and manage timed broadcast messages visible in the GUI.
 *
 * Timing for broadcasts uses std::chrono::steady_clock to ensure consistent
 * durations unaffected by system time changes.
 */
 
/*
** EPITECH PROJECT, 2025
** ZappyGUI
** File description:
** Player.cpp
*/

#include "Player.hpp"

/**
 * @brief Get the X coordinate of the player
 *
 * Returns the current X position of the player on the game map.
 * This coordinate is typically used to determine the player's
 * horizontal position in a 2D grid-based environment.
 *
 * @return std::size_t The X coordinate of the player
 *
 * @note The coordinate is stored internally and does not change
 *       unless explicitly updated by movement or game logic.
 */
std::size_t GUI::Player::getX() const
{
    return _x;
}

/**
 * @brief Get the Y coordinate of the player
 *
 * Returns the current Y position of the player on the game map.
 * This coordinate represents the player's vertical position in a
 * 2D grid-based environment.
 *
 * @return std::size_t The Y coordinate of the player
 *
 * @note The value is determined by the game logic and only changes
 *       when the player moves or the game state is updated.
 */
std::size_t GUI::Player::getY() const
{
    return _y;
}

/**
 * @brief Get the name of the player
 *
 * Returns a constant reference to the player's name.
 * The name is typically assigned when the player is created
 * and can be used for identification, display, or logging purposes.
 *
 * @return const std::string& The name of the player
 *
 * @note The returned reference is constant and should not be modified.
 * @note The lifetime of the returned string is managed by the Player object.
 */
const std::string &GUI::Player::getName() const
{
    return _name;
}

/**
 * @brief Get the team name of the player
 *
 * Returns a constant reference to the name of the team to which
 * the player belongs. Team names are used to group players for
 * gameplay logic, rendering, or scoring.
 *
 * @return const std::string& The name of the player's team
 *
 * @note The returned reference must not be modified.
 * @note The string is owned by the Player instance.
 */
const std::string &GUI::Player::getTeam() const
{
    return _team;

}

/**
 * @brief Get the orientation of the player
 *
 * Returns the current orientation (facing direction) of the player
 * as an integer value. The orientation typically corresponds to a
 * direction on the game map (e.g., 1 = North, 2 = East, 3 = South, 4 = West).
 *
 * @return int The player's orientation as an integer value
 *
 * @note The meaning of the orientation values depends on the game protocol.
 * @note This value is usually updated based on player movement or actions.
 */
int GUI::Player::getOrientation() const 
{
    return _orientation;
}

/**
 * @brief Get the current level of the player
 *
 * Returns the player's current level, which typically represents
 * their progress, experience, or rank within the game.
 *
 * @return int The level of the player
 *
 * @note The level is managed by the game logic and may increase
 *       as the player completes objectives or gains experience.
 */
int GUI::Player::getLevel() const
{ 
    return _level;
}

/**
 * @brief Get the player's current broadcast message
 *
 * Returns a constant reference to the last broadcast message
 * sent or received by the player. Broadcast messages are typically
 * used for communication between players within the game.
 *
 * @return const std::string& The current broadcast message
 *
 * @note The message content is managed by the game logic.
 * @note The returned reference must not be modified directly.
 */
const std::string &GUI::Player::getBroadcastMessage() const
{
    return _broadcastMessage;
}

/**
 * @brief Set the level of the player
 *
 * Updates the player's current level to the specified value.
 * This method is typically called by game logic to reflect
 * progress, experience gains, or event triggers.
 *
 * @param lvl The new level to assign to the player
 *
 * @note The level should be a positive integer defined by game rules.
 */
void GUI::Player::setLevel(int lvl)
{
    _level = lvl;
}

/**
 * @brief Set the position of the player on the map
 *
 * Updates the player's current coordinates to the specified X and Y values.
 * This method is typically used to move the player or initialize their
 * position during game setup or synchronization with the server state.
 *
 * @param x The new X coordinate of the player
 * @param y The new Y coordinate of the player
 *
 * @note Coordinates are usually grid-based and should be within
 *       the bounds of the game map.
 */
void GUI::Player::setPosition(std::size_t x, std::size_t y)
{
    _x = x;
    _y = y;
}
        
/**
 * @brief Add a movement offset to the player's current move
 *
 * Increments the player's pending movement by the given x and y offsets.
 * This can be used to accumulate movement commands before applying them.
 *
 * @param x The movement offset along the X axis
 * @param y The movement offset along the Y axis
 */
void GUI::Player::addMove(int x, int y)
{
    _moveX = _moveX + x;
    _moveY = _moveY + y;
}

/**
 * @brief Set the player's inventory
 *
 * Updates the player's inventory with the provided vector of item counts or IDs.
 *
 * @param inventory A constant reference to the vector representing the inventory
 */
void GUI::Player::setInventory(const std::vector<int> &inventory)
{
    _inventory = inventory;
}

/**
 * @brief Set the orientation of the player
 *
 * Updates the player's current orientation (facing direction).
 *
 * @param o The new orientation value (e.g., 1 = North, 2 = East, etc.)
 */
void GUI::Player::setOrientation(int o) {
    _orientation = o;
}

/**
 * @brief Set the broadcast message for the player
 *
 * Updates the player's current broadcast message and resets the broadcast timer.
 * Broadcast messages are used for in-game communication visible for a limited time.
 *
 * @param message The new broadcast message to set
 */
void GUI::Player::setBroadcastMessage(const std::string &message)
{
    _broadcastMessage = message;
    _broadcastStartTime = std::chrono::steady_clock::now();
}

/**
 * @brief Check if the broadcast message should still be shown
 *
 * Determines whether the player's broadcast message is still within the allowed
 * display duration and should be shown to other players.
 *
 * @return true if the broadcast message is active and visible
 * @return false if there is no message or it has expired
 */
[[nodiscard]] bool GUI::Player::shouldShowBroadcast() const
{
    if (_broadcastMessage.empty())
        return false;
            
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - _broadcastStartTime);
    return duration.count() < BROADCAST_DURATION;
}

/**
 * @brief Clear the broadcast message if its display duration has expired
 *
 * Checks if the broadcast message should no longer be shown and clears it if expired.
 */
void GUI::Player::clearBroadcastIfExpired()
{
    if (!shouldShowBroadcast())
        _broadcastMessage.clear();
}
