#include <cstddef>
#include <string>
#include <vector>

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
        : _x(0), _y(0), _orientation(0), _level(1) {}

        Player(std::size_t x, std::size_t y, std::string name, std::string team, int orientation, int level = 1)
        : _x(x), _y(y), _name(std::move(name)), _team(std::move(team)), _orientation(orientation), _level(level) {}

        [[nodiscard]] std::size_t getX() const { return _x; }
        [[nodiscard]] std::size_t getY() const { return _y; }
        [[nodiscard]] const std::string &getName() const { return _name; }
        [[nodiscard]] const std::string &getTeam() const { return _team; }
        [[nodiscard]] int getorientation() const { return _orientation; }
        [[nodiscard]] int getLevel() const { return _level; }
        void setLevel(int lvl) { _level = lvl; }

        void setPosition(std::size_t x, std::size_t y) {
            _x = x;
            _y = y;
        }
        void setInventory(const std::vector<int> &inventory) {
            _inventory = inventory;
        }

        void setOrientation(int o) { _orientation = o; }

    private:
        std::size_t _x;
        std::size_t _y;
        std::string _team;
        std::string _name;
        int _orientation;
        int _level;
        std::vector<int> _inventory;
    };
} // namespace GUI
