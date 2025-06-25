/*
** EPITECH PROJECT, 2025
** ZappyGUI
** File description:
** Clock.cpp
*/

#include "Clock.hpp"

GUI::Clock::Clock() : _timeUnit(100)
{
    _lastTick = std::chrono::steady_clock::now();
}

void GUI::Clock::setTimeUnit(int timeUnit)
{
    _timeUnit = timeUnit;
}

double GUI::Clock::getElapsedSeconds() const
{
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = now - _lastTick;
    return duration.count();
}