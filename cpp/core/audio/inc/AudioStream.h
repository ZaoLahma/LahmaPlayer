#pragma once

#include "AudioSource.h"

#include <memory>
#include <portaudio.h>
#include <functional>

namespace LahmaPlayer::AudioStream
{
    /**
    * @class AudioStream
    * @brief AudioStream is a class that is used to stream audio data
    */
    class AudioStream
    {
    public:
        AudioStream();
        ~AudioStream();

        void start(std::shared_ptr<AudioSource::AudioSource> audioSource);
        void stop();

        void waitUntilFinished();

        bool isPlaying() const { return m_playing && m_stream; }
        
        /**
         * @brief Set callback for when playback finishes
         * @param callback Function to call when current track finishes
         */
        void setOnPlaybackFinishedCallback(std::function<void()> callback)
        {
            m_onFinishedCallback = callback;
        }

    private:
        //PortAudio callback
        static int callbackStatic(const void *inputBuffer, void *outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo *timeInfo,
                                  PaStreamCallbackFlags statusFlags,
                                  void *userData);

        int callback(void *outputBuffer, unsigned long frameCount);

        std::shared_ptr<AudioSource::AudioSource> m_audioSource;
        AudioSource::AudioSource::AudioFormat m_audioFormat;
        PaStream* m_stream;
        bool m_playing = false;
        std::function<void()> m_onFinishedCallback;
    };
}