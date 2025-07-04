/*
** EPITECH PROJECT, 2025
** ZappyGUI
** File description:
** Player.cpp
*/

#include "Player.hpp"

std::size_t GUI::Player::getX() const
{
    return _x;
}

std::size_t GUI::Player::getY() const
{
    return _y;
}

const std::string &GUI::Player::getName() const
{
    return _name;
}

const std::string &GUI::Player::getTeam() const
{
    return _team;

}
int GUI::Player::getOrientation() const 
{
    return _orientation;
}

int GUI::Player::getLevel() const
{ 
    return _level;
}

const std::string &GUI::Player::getBroadcastMessage() const
{
    return _broadcastMessage;
}

void GUI::Player::setLevel(int lvl)
{
    _level = lvl;
}

void GUI::Player::setPosition(std::size_t x, std::size_t y)
{
    _x = x;
    _y = y;
}
        
void GUI::Player::addMove(int x, int y)
{
    _moveX = _moveX + x;
    _moveY = _moveY + y;
}
        
void GUI::Player::setInventory(const std::vector<int> &inventory)
{
    _inventory = inventory;
}

void GUI::Player::setOrientation(int o) {
    _orientation = o;
}

void GUI::Player::setBroadcastMessage(const std::string &message)
{
 _broadcastMessage = message;
    _broadcastStartTime = std::chrono::steady_clock::now();
}

[[nodiscard]] bool GUI::Player::shouldShowBroadcast() const
{
    if (_broadcastMessage.empty())
        return false;
            
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - _broadcastStartTime);
    return duration.count() < BROADCAST_DURATION;
}

void GUI::Player::clearBroadcastIfExpired()
{
    if (!shouldShowBroadcast())
        _broadcastMessage.clear();
}