#include "AudioStream.h"
#include <cmath>
#include <portaudio.h>

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

    void AudioStream::start(std::shared_ptr<AudioFile::AudioFile> audioFile)
    {
        m_audioFile = audioFile;
        m_playing = true;

        Pa_Initialize();
        Pa_OpenDefaultStream(&m_stream, 0, 1, paFloat32, 44100, 256,
                           &AudioStream::callbackStatic, this);
        Pa_StartStream(m_stream);
    }

    void AudioStream::stop()
    {
        if (m_playing)
        {
            Pa_StopStream(m_stream);
            Pa_CloseStream(m_stream);
        }
        m_playing = false;
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
        static float phase = 0.0f;
        float *out = (float*)output;
        
        for (unsigned long i = 0; i < frameCount; i++) {
            out[i] = 0.3f * sin(phase);  // 440 Hz sine wave
            phase += 2.0f * M_PI * 440.0f / 44100.0f;
            if (phase > 2.0f * M_PI) phase -= 2.0f * M_PI;
        }
        
        return paContinue;
    }
}