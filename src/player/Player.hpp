#include <cstddef>
#include <string>
#include <vector>

enum orientation {
    North,
    East,
    South,
    West,
};

namespace GUI {
    class Player {
        private:
            std::size_t _x;
            std::size_t _y;
            std::string _team;
            std::string _name;
            orientation _o;
    };
} // namespace GUI
