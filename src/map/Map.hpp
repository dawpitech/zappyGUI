#include <array>
#include <cstddef>
#include <vector>
#include "../resource/IResource.hpp"

namespace GUI {
    template <std::size_t width, std::size_t height>
    class Map {
        private:
            std::array<std::array<std::vector<IResource>, height>, width> _grid;
            std::size_t _width;
            std::size_t _height;
        public:
            [[nodiscard]] constexpr std::size_t get_width() const { return width; }
            [[nodiscard]] constexpr std::size_t get_height() const { return height; }
    };
} // namespace GUI
