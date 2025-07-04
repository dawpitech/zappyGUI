/*
** EPITECH PROJECT, 2025
** ZappyGUI
** File description:
** Player.hpp
*/

#include <cstddef>
#include <string>
#include <vector>
#include <chrono>
#include "../macros.hpp"

namespace GUI {
    class Player {
    public:
        Player()
        : _x(0), _y(0), _orientation(0), _level(1), _broadcastStartTime(std::chrono::steady_clock::now()) {}

        Player(std::size_t x, std::size_t y, std::string name, std::string team, int orientation, int level = 1)
        : _x(x), _y(y), _name(std::move(name)), _team(std::move(team)), _orientation(orientation), _level(level), 
         _broadcastStartTime(std::chrono::steady_clock::now()) {}

        [[nodiscard]] std::size_t getX() const;
        [[nodiscard]] std::size_t getY() const;
        [[nodiscard]] const std::string &getName() const;
        [[nodiscard]] const std::string &getTeam() const;
        [[nodiscard]] int getOrientation() const;
        [[nodiscard]] int getLevel() const;
        [[nodiscard]] const std::string &getBroadcastMessage() const;
        
        void setLevel(int lvl);
        void setPosition(std::size_t x, std::size_t y);
        void setInventory(const std::vector<int> &inventory);
        void setBroadcastMessage(const std::string &message);
        void setOrientation(int o);

        void addMove(int x, int y);

        [[nodiscard]] bool shouldShowBroadcast() const;

        void clearBroadcastIfExpired();

    private:
        std::size_t _x;
        std::size_t _y;
        std::string _team;
        std::string _name;
        int _orientation;
        int _level;
        std::vector<int> _inventory;
        int _moveX = 0;
        int _moveY = 0;
        std::string _broadcastMessage;
        std::chrono::steady_clock::time_point _broadcastStartTime;
    };
} // namespace GUI
