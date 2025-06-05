#include "IResource.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace GUI {
    class Player {
        private:
            std::vector<IResource> _inventory;
            std::size_t _x;
            std::size_t _y;
            std::string _team;
            std::string _name;
    };
} // namespace GUI
