/*
** EPITECH PROJECT, 2025
** ZappyGUI
** File description:
** Audio.hpp
*/

#pragma once

#include "../../include/raylib-cpp.hpp"
#include <map>
#include <string>
#include <iostream>

namespace GUI {
    class AudioManager {
    public:
        class AudioError : public std::exception {
                    private:
                        std::string _message;
                    public:
                        AudioError(std::string  message) : _message(std::move(message)) {}
                        [[nodiscard]] const char* what() const noexcept override { return _message.c_str(); }
            };
        AudioManager();
        ~AudioManager();

        void play(const std::string &name);

    private:
        std::map<std::string, raylib::Sound> _sounds;
    };
    
} // namespace GUI
