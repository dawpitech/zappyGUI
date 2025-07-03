#pragma once

#include <map>
#include <string>
#include "../../include/raylib-cpp.hpp"

namespace GUI {
    class AudioManager {
    public:
        AudioManager();
        ~AudioManager() {
            CloseAudioDevice();
        };

        void play(const std::string &name);

    private:
        std::map<std::string, raylib::Sound> _sounds;
    };
} // namespace GUI
