#include "Audio.hpp"

GUI::AudioManager::AudioManager() {
    InitAudioDevice();
    _sounds.emplace("broadcast", raylib::Sound("assets/raaaah.wav"));
    _sounds.emplace("deathPlayer", raylib::Sound("assets/deathPlayer.wav"));
}

void GUI::AudioManager::play(const std::string &name) {
    auto it = _sounds.find(name);
    if (it != _sounds.end()) {
        it->second.Play();
    }
}
