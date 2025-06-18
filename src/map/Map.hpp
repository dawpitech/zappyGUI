#include <array>
#include <cstddef>
#include <vector>
#include "../resource/IResource.hpp"
#include "../../include/raylib-cpp.hpp"

namespace GUI {
    template <std::size_t width, std::size_t height>
    class Map {
        private:
            std::array<std::array<std::vector<IResource>, height>, width> _grid;
            float _tileSize;
        public:
            Map(float tileSize = 1.0f);
            [[nodiscard]] constexpr std::size_t get_width() const { return width; }
            [[nodiscard]] constexpr std::size_t get_height() const { return height; }

            void drawGround();
            void drawResources();
            void drawEggs();
            void drawPlayers();


            void render();
    };
} // namespace GUI
