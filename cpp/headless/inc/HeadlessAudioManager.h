#pragma once

#include "AudioManager.h"
#include <memory>

namespace LahmaPlayer::Headless {

/**
 * @class HeadlessAudioManager
 * @brief Simple headless audio playback manager
 * Plays audio files until completion.
 */
class HeadlessAudioManager
{
public:
    HeadlessAudioManager();
    ~HeadlessAudioManager();

    /**
     * @brief Load and play an audio file
     * @param fileName The path to the audio file
     * @return true if successful, false otherwise
     */
    bool playFile(const std::string& fileName);

    /**
     * @brief Stop playback
     */
    void stop();

private:
    std::unique_ptr<Gui::AudioManager> m_audioManager;
};

} // namespace LahmaPlayer::Headless
