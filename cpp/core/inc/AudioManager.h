#pragma once

#include "AudioSource.h"
#include "AudioSourceFactory.h"
#include "AudioStream.h"
#include "DspEngine.h"
#include "Logger.h"
#include <functional>
#include <memory>
#include <string>

namespace LahmaPlayer::Gui
{
class AudioManager
{
  public:
    AudioManager();
    ~AudioManager();

    /**
     * @brief Load an audio file
     * @param fileName The name of the file to load
     * @return true if successful, false otherwise
     */
    bool loadAudioFile(const std::string &fileName);

    /**
     * @brief Start playback of the loaded audio file
     */
    void startPlayback();

    /**
     * @brief Stop playback of the loaded audio file
     */
    void stopPlayback();

    /**
     * @brief Get the audio source
     * @return Pointer to the audio source
     */
    std::shared_ptr<LahmaPlayer::AudioSource::AudioSource> getAudioSource() const
    {
        return m_audioSource;
    }

    /**
     * @brief Get the audio stream
     * @return Pointer to the audio stream
     */
    std::shared_ptr<LahmaPlayer::AudioStream::AudioStream> getAudioStream() const
    {
        return m_audioStream;
    }

    /**
     * @brief Get the DSP engine
     * @return Pointer to the DSP engine
     */
    std::shared_ptr<LahmaPlayer::DspEngine::DspEngine> getDspEngine() const
    {
        return m_dspEngine;
    }

    /**
     * @brief Seek to a new position
     * @param numSamples Number of samples to seek
     * @param direction Seek direction (Forward or Backward)
     * @return true if seek was successful
     */
    bool seek(uint32_t numSamples, LahmaPlayer::AudioSource::AudioSource::SeekDirection direction)
    {
        if (m_audioSource)
        {
            // Get current position before seeking
            uint32_t currentPosition = m_audioSource->getTotalSamples();
            
            m_audioSource->seek(numSamples, direction);
            
            // Call seek callback with new position
            if (m_onSeekCallback)
            {
                m_onSeekCallback(m_audioSource->getTotalSamples());
            }
            
            return true;
        }
        return false;
    }

    /**
     * @brief Get total samples in current track
     * @return Total number of samples
     */
    uint32_t getTotalSamples() const
    {
        return m_audioSource ? m_audioSource->getTotalSamples() : 0;
    }

    /**
     * @brief Set callback for when seek occurs
     * @param callback Function to call when seek happens
     */
    void setOnSeekCallback(std::function<void(uint32_t position)> callback);

    /**
     * @brief Set callback for when playback finishes
     * @param callback Function to call when current track finishes
     */
    void setOnPlaybackFinishedCallback(std::function<void()> callback);

    /**
     * @brief Check if currently playing
     * @return true if playback is active
     */
    bool isPlaying() const;

  private:
    // Audio resources
    std::shared_ptr<LahmaPlayer::AudioSource::AudioSource> m_audioSource;
    std::shared_ptr<LahmaPlayer::DspEngine::DspEngine> m_dspEngine;
    std::shared_ptr<LahmaPlayer::AudioStream::AudioStream> m_audioStream;

    // Playback state
    std::function<void()> m_onPlaybackFinishedCallback;
    std::function<void(uint32_t position)> m_onSeekCallback;
    bool m_isPlaying = false;
};
} // namespace LahmaPlayer::Gui
