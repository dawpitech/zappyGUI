/**
 * @file Clock.cpp
 * @brief Implementation of the Clock class for the ZappyGUI project
 * @author EPITECH PROJECT, 2025
 * @date 2025
 *
 * This file contains the implementation of the Clock class which provides
 * time management and measurement functionality for the ZappyGUI application.
 * The class handles time unit configuration and elapsed time calculation
 * using high-resolution steady clock for accurate timing measurements.
 *
 * The clock is designed to track elapsed time from a reference point and
 * manage time units for game synchronization and timing operations.
 */

/*
** EPITECH PROJECT, 2025
** ZappyGUI
** File description:
** Clock.cpp
*/

#include "Clock.hpp"

/**
 * @brief Constructs a new Clock object with default settings
 *
 * Initializes the clock with a default time unit of 100 and sets the
 * reference time point to the current moment using std::chrono::steady_clock.
 * The steady clock is used to ensure monotonic time progression that is
 * not affected by system clock adjustments.
 *
 * The default time unit of 100 is commonly used in game timing systems
 * and can be adjusted later using setTimeUnit().
 *
 * @note The constructor captures the current time as the reference point
 * @note std::chrono::steady_clock provides high-resolution timing
 * @note The default time unit (100) can be modified after construction
 */
GUI::Clock::Clock() : _timeUnit(100)
{
    _lastTick = std::chrono::steady_clock::now();
}

/**
 * @brief Sets the time unit for the clock
 *
 * Configures the time unit value used by the clock for timing calculations.
 * The time unit typically represents the number of time units per second
 * or a scaling factor for time-based operations in the application.
 *
 * This method allows dynamic adjustment of timing parameters during runtime,
 * which is useful for game speed control, synchronization with server timing,
 * or adapting to different performance requirements.
 *
 * @param timeUnit The new time unit value to set
 *
 * @note The time unit affects how time-based calculations are performed
 * @note Common values might include frames per second, ticks per second, etc.
 * @note This method does not reset the reference time point
 */
void GUI::Clock::setTimeUnit(int timeUnit)
{
    _timeUnit = timeUnit;
}

/**
 * @brief Calculates the elapsed time since the last reference point
 *
 * Computes the time difference between the current moment and the stored
 * reference time point (_lastTick). The result is returned as a floating-point
 * number representing seconds with high precision.
 *
 * This method uses std::chrono::steady_clock for consistent timing that is
 * not affected by system clock adjustments or time zone changes. The
 * calculation provides sub-second precision suitable for game timing,
 * animation, and performance measurement.
 *
 * @return double The elapsed time in seconds since the reference point
 *
 * @note The return value is in seconds with floating-point precision
 * @note Uses std::chrono::steady_clock for monotonic time measurement
 * @note The reference time point is not modified by this method
 * @note Multiple calls will return increasing values as time progresses
 *
 * @see std::chrono::steady_clock for details on the timing mechanism
 */
double GUI::Clock::getElapsedSeconds() const
{
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = now - _lastTick;
    return duration.count();
}
