#pragma once

#include "AudioFile.h"

#include <memory>
#include <portaudio.h>

namespace LahmaPlayer::AudioStream
{
    class AudioStream
    {
    public:
        AudioStream();
        ~AudioStream();

        void start(std::shared_ptr<AudioFile::AudioFile> audioFile);
        void stop();

        void waitUntilFinished();

    private:
        //PortAudio callback
        static int callbackStatic(const void *inputBuffer, void *outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo *timeInfo,
                                  PaStreamCallbackFlags statusFlags,
                                  void *userData);

        int callback(void *outputBuffer, unsigned long frameCount);

        std::shared_ptr<AudioFile::AudioFile> m_audioFile;
        AudioFile::AudioFile::AudioFileFormat m_audioFormat;
        PaStream* m_stream;
        bool m_playing = false;
    };
}