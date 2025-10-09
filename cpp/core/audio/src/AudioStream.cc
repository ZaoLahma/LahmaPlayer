#include "AudioStream.h"
#include <cmath>
#include <portaudio.h>
#include <thread>

namespace LahmaPlayer::AudioStream
{
    AudioStream::AudioStream()
    {

    }

    AudioStream::~AudioStream()
    {
        if (m_playing)
        {
            stop();
        }
        Pa_Terminate();
    }

    void AudioStream::start(std::shared_ptr<AudioSource::AudioSource> audioSource)
    {
        m_audioSource = audioSource;
        m_playing = true;

        Pa_Initialize();
        m_audioFormat = m_audioSource->getAudioFormat();
        Pa_OpenDefaultStream(&m_stream, 0, m_audioFormat.numChannels, paFloat32, m_audioFormat.sampleRate, 256,
                           &AudioStream::callbackStatic, this);
        Pa_StartStream(m_stream);
    }

    void AudioStream::stop()
    {
        if (nullptr != m_stream)
        {
            Pa_StopStream(m_stream);
            Pa_CloseStream(m_stream);
        }

        m_stream = nullptr;
    }
    
    void AudioStream::waitUntilFinished()
    {
        while (m_playing)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    int AudioStream::callbackStatic(
        const void *input, void *output,
        unsigned long frameCount,
        const PaStreamCallbackTimeInfo *timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData)
    {
        AudioStream *player = (AudioStream*)userData;
        return player->callback(output, frameCount);
    }

    int AudioStream::callback(void *output, unsigned long frameCount)
    {
        static thread_local std::vector<float> samples;
        samples.resize(frameCount * m_audioFormat.numChannels);
        m_audioSource->read(samples, frameCount * m_audioFormat.numChannels);

        float *out = (float*)output;
        
        for (unsigned long i = 0; i < frameCount * m_audioFormat.numChannels; i++)
        {
            // Assume interleaved L / R channels if stereo
            out[i] = samples[i];
        }
        
        if (m_audioSource->hasMore())
        {
            return paContinue;
        }

        m_playing = false;
        return paComplete;
    }
}