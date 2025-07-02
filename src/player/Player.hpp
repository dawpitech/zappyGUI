#include <cstddef>
#include <string>
#include <vector>
#include <chrono>

// enum orientation {
//     North = 1,
//     East = 2,
//     South = 3,
//     West = 4,
// };

namespace GUI {
    class Player {
    public:
        Player()
        : _x(0), _y(0), _orientation(0), _level(1), _broadcastStartTime(std::chrono::steady_clock::now()) {}

        Player(std::size_t x, std::size_t y, std::string name, std::string team, int orientation, int level = 1)
        : _x(x), _y(y), _name(std::move(name)), _team(std::move(team)), _orientation(orientation), _level(level), 
         _broadcastStartTime(std::chrono::steady_clock::now()) {}

        [[nodiscard]] std::size_t getX() const { return _x; }
        [[nodiscard]] std::size_t getY() const { return _y; }
        [[nodiscard]] const std::string &getName() const { return _name; }
        [[nodiscard]] const std::string &getTeam() const { return _team; }
        [[nodiscard]] int getorientation() const { return _orientation; }
        [[nodiscard]] int getLevel() const { return _level; }
        [[nodiscard]] const std::string &getBroadcastMessage() const { return _broadcastMessage; }
        
        void setLevel(int lvl) { _level = lvl; }

        void setPosition(std::size_t x, std::size_t y) {
            _x = x;
            _y = y;
        }
        
        void addMove(int x, int y) {
            _moveX = _moveX + x;
            _moveY = _moveY + y;
        }
        
        void setInventory(const std::vector<int> &inventory) {
            _inventory = inventory;
        }

        void setOrientation(int o) { _orientation = o; }

        void setBroadcastMessage(const std::string &message) {
            _broadcastMessage = message;
            _broadcastStartTime = std::chrono::steady_clock::now();
        }

        [[nodiscard]] bool shouldShowBroadcast() const {
            if (_broadcastMessage.empty()) return false;
            
            auto now = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - _broadcastStartTime);
            return duration.count() < 3;
        }

        void clearBroadcastIfExpired() {
            if (!shouldShowBroadcast()) {
                _broadcastMessage.clear();
            }
        }

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