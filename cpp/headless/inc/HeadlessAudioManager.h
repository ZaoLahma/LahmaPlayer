#pragma once

#include "AudioManager.h"
#include <memory>

namespace LahmaPlayer::Headless
{

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
    bool playFile(const std::string &fileName);

    /**
     * @brief Stop playback
     */
    void stop();

    /**
     * @brief Seek to absolute sample position
     * @param samples Absolute sample position to seek to
     */
    void seekTo(uint32_t samples);

    /**
     * @brief Seek forward by number of samples
     * @param numSamples Number of samples to seek forward
     */
    void seekForward(uint32_t numSamples);

    /**
     * @brief Seek backward by number of samples
     * @param numSamples Number of samples to seek backward
     */
    void seekBackward(uint32_t numSamples);

  private:
    std::unique_ptr<Gui::AudioManager> m_audioManager;
};

} // namespace LahmaPlayer::Headless
