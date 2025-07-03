/**
 * @file Audio.cpp
 * @brief Implementation of the AudioManager class for the ZappyGUI project
 * @author EPITECH PROJECT, 2025
 * @date 2025
 *
 * This file contains the implementation of the AudioManager class which handles
 * audio system management and sound playback for the ZappyGUI application.
 * The class provides centralized audio control, sound loading, and playback
 * functionality using the Raylib audio system.
 *
 * The audio manager maintains a collection of pre-loaded sounds that can be
 * played on demand by name, providing an efficient sound management system
 * for game events and user interface feedback.
 */

#include "Audio.hpp"

/**
 * @brief Constructs a new AudioManager object and initializes the audio system
 *
 * Initializes the Raylib audio device and loads all required sound assets
 * into memory. The constructor sets up the audio system and populates the
 * sound collection with predefined audio files.
 *
 * Currently loaded sounds include:
 * - "broadcast": A dramatic sound effect (raaaah.wav) for broadcast events
 * - "deathPlayer": A death sound effect (deathPlayer.wav) for player elimination
 *
 * The audio device initialization is handled by Raylib's InitAudioDevice()
 * which sets up the audio context and prepares the system for sound playback.
 *
 * @note The constructor will attempt to load all audio files from the assets directory
 * @note If audio files are missing, the sound objects may not function correctly
 * @note The audio device must be properly initialized before any sound operations
 * @note Memory for sound assets is allocated during construction
 *
 * @throws May throw exceptions if audio device initialization fails
 * @throws May throw exceptions if sound file loading fails
 */
GUI::AudioManager::AudioManager() {
    InitAudioDevice();
    _sounds.emplace("broadcast", raylib::Sound("assets/raaaah.wav"));
    _sounds.emplace("deathPlayer", raylib::Sound("assets/deathPlayer.wav"));
}

/**
 * @brief Plays a sound by name
 *
 * Searches for a sound with the specified name in the loaded sound collection
 * and plays it if found. The method performs a safe lookup to ensure the
 * sound exists before attempting playback.
 *
 * Available sound names:
 * - "broadcast": Plays the broadcast event sound effect
 * - "deathPlayer": Plays the player death sound effect
 *
 * If the specified sound name is not found in the collection, the method
 * silently returns without performing any action, ensuring robust error
 * handling without crashing the application.
 *
 * @param name The name identifier of the sound to play
 *
 * @note The method performs case-sensitive string matching
 * @note If the sound name doesn't exist, no error is reported
 * @note Multiple simultaneous playbacks of the same sound are supported
 * @note The method is safe to call with invalid sound names
 * @note Sound playback is asynchronous and non-blocking
 *
 * @see AudioManager() for the list of available sound names
 *
 * @par Example Usage:
 * @code
 * audioManager.play("broadcast");     // Plays broadcast sound
 * audioManager.play("deathPlayer");   // Plays death sound
 * audioManager.play("nonexistent");   // Safe, does nothing
 * @endcode
 */
void GUI::AudioManager::play(const std::string &name) {
    auto it = _sounds.find(name);
    if (it != _sounds.end()) {
        it->second.Play();
    }
}
