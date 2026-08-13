#pragma once

#include "AudioSource.h"
#include "AudioStream.h"
#include "DspEngine.h"
#include "AudioSourceFactory.h"
#include "Logger.h"
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
        bool loadAudioFile(const std::string& fileName);

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
        bool m_isPlaying = false;
    };
}