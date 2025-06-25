/*
** EPITECH PROJECT, 2025
** ZappyGUI
** File description:
** Clock.hpp
*/

#pragma once

#include <chrono>

namespace GUI {
    class Clock {
    public:
        Clock();
        void setTimeUnit(int timeUnit);
        [[nodiscard]] double getElapsedSeconds() const;
    private:
        int _timeUnit;
        std::chrono::steady_clock::time_point _lastTick;
    };
}